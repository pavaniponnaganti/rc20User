/** @file noja_webserver_pkg.c
 * Package handling implementations
 *
 * @version "$"
 */
/*
 * LIMITATIONS
 *
 * This document is copyright © NOJA Power Switchgear Pty Ltd 2020. It contains
 * confidential intellectual property that belongs to NOJA Power Switchgear Pty
 * Ltd. It does NOT invest any rights to that intellectual property in the
 * recipient.
 *
 * This document is provided solely for limited use by the recipient, who is
 * NOT permitted in any way to copy, or to disclose to any other party, any
 * part of the contents of this document directly or indirectly without the
 * express written permission of NOJA Power Switchgear Pty Ltd.
 */
#include <sys/types.h>
#include <sys/stat.h>
#if !defined(_MSC_VER)
#include <unistd.h>
#endif
#include "noja_webserver_pkg.h"
#include "noja_webserver_mem.h"
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "libxml/xpath.h"
#include "libxml/xpathInternals.h"


/** @ingroup group_internal
  * @brief define a static mime type for FTL file
  */
static struct lws_protocol_vhost_options nwsFtlMime = {
        NULL, NULL, ".ftl", "text/ftl"
};

/** @ingroup group_internal
  * @brief Count the number of element matching the specified xpath
  *
  * This function get the number of elements matching an expath string
  *
  * @param[in] ctx Webserver context
  * @param[in] xpathCtx XML file where to do the search
  * @param[in] xpath String xpath use to search for the elements
  * @param[out] count Number of matching elements
  * @return NwsError_Ok if no error occured
  * @return NwsError_ConfigurationFile Failed to find the specified object.
  */
static NwsErrors nswFindMatchedCount(NwsContextInternal *ctx, xmlXPathContextPtr xpathCtx, const char* xpath, unsigned int* count)
{
    xmlXPathObjectPtr xpathMatch;
    NwsErrors result = NwsError_Ok;

    PARAM_UNUSED(ctx);

    /* match the xpath to the configuration fi */
    if((xpathMatch = xmlXPathEvalExpression((const xmlChar *)xpath, xpathCtx)) == NULL) {
        SYSERR(SYS_INFO, "Unable to evaluate the xpath expression \"%s\"", xpath);
        return NwsError_ConfigurationFile;
    }

    if(xmlXPathNodeSetIsEmpty(xpathMatch->nodesetval))  {
        SYSERR(SYS_INFO, "No match for the xpath expression \"%s\"", xpath);
        result = NwsError_ConfigurationFile;
    } else {
         *count = xpathMatch->nodesetval->nodeNr;
    }
    xmlXPathFreeObject(xpathMatch);

    return result;
}
/** @ingroup group_internal
  * @brief Find the specified element or attribute and get the text content
  *
  * This function search the specified element/attribute using XPATH. The searched will get the first
  * match and ignoring the other match. Will only get string for XML text node and XML attribute.
  *
  * @param[in] ctx Webserver context
  * @param[in] xpathCtx XML file where to do the search
  * @param[in] xpath String xpath use to search for the element
  * @param[out] text pointer to the element's text content
  * @return NwsError_Ok if no error occured
  * @return NwsError_ConfigurationFile Failed to find the specified object.
  */
static NwsErrors nswFindElementGetText(NwsContextInternal *ctx, xmlXPathContextPtr xpathCtx, const char* xpath, char** text)
{
    xmlXPathObjectPtr xpathMatch;
    xmlNode *current = NULL;
    NwsErrors result = NwsError_Ok;

    /* match the xpath to the configuration fi */
    if((xpathMatch = xmlXPathEvalExpression((const xmlChar *)xpath, xpathCtx)) == NULL) {
        SYSERR(SYS_INFO, "Unable to evaluate the xpath expression \"%s\"", xpath);
        return NwsError_ConfigurationFile;
    }

    do {
        if(xmlXPathNodeSetIsEmpty(xpathMatch->nodesetval))  {
            SYSERR(SYS_INFO, "No match for the xpath expression \"%s\"", xpath);
            result = NwsError_ConfigurationFile;
            break;
        }
        /* there should be one default element */
        current = xpathMatch->nodesetval->nodeTab[0];

        /* if node is attribute, the value is in the node from 'children' field */
        if(current->type == XML_ATTRIBUTE_NODE ) {
            xmlAttr *attr = (xmlAttr*)current;
            current = attr->children;
        }

        if( current->type == XML_TEXT_NODE) {
            *text = (char*)NWSALLOC(ctx, strbuflen(current->content));
            memset(*text, 0, strbuflen(current->content));
            strncpy(*text, (const char*)current->content, strlen((const char*)current->content));
        } else {
            SYSERR(SYS_INFO, "Unable to get the string value for xpath expression \"%s\"", xpath);
            result = NwsError_ConfigurationFile;
            break;
        }
    } while (0);
    xmlXPathFreeObject(xpathMatch);

    return result;
}
/** @ingroup group_internal
  * @brief Find the specified element/attribute and get the text content as number
  *
  * This function search the specified element using XPATH. The searched will get the first
  * match and ignoring the other match. Will only get string for XML text node and XML attribute.
  *
  * @param[in] ctx Webserver context
  * @param[in] xpathCtx XML file where to do the search
  * @param[in] xpath String xpath use to search for the element
  * @param[out] text pointer to the element's text content
  * @return NwsError_Ok if no error occured
  * @return NwsError_ConfigurationFile Failed to find the specified object.
  */
static NwsErrors nswFindElementGetNumber(NwsContextInternal *ctx, xmlXPathContextPtr xpathCtx, const char* xpath, unsigned int* num)
{
    char *text = NULL;
    char *endptr = NULL;
    NwsErrors result = NwsError_Ok;

    if((result = nswFindElementGetText(ctx, xpathCtx, xpath, &text)) != NwsError_Ok) {
        return result;
    }
    /* convert the value into unsinged int */
    *num = strtol(text, &endptr, 10);
    if(text == endptr) {
        NWSFREE(ctx, text, strbuflen(text));
        SYSERR(SYS_INFO, "The value of the expression \"%s\" is not a number", xpath);
        result = NwsError_ConfigurationFile;
    }
    NWSFREE(ctx, text, strbuflen(text));
    return result;
}
/** @ingroup group_internal
  * @brief Get the languages from the configuration file
  *
  * This function parse the language resources from the configuration file
  *
  * @param[in] ctx Webserver context
  * @param[in] xpathCtx Configuration file where to search the languages
  * @param[in] mount Pointer to the mount structure where to add the language resource
  * @param[in, out] idx index of mount, this is populated on return with the final index used
  * @return NwsError_Ok if no error occured
  * @return NwsError_ConfigurationFile Failed to parse the configuration file. Check the log for details
  * @return NwsError_Ng General error
  */
static NwsErrors nwsParseLangs(NwsContextInternal *ctx, xmlXPathContextPtr xpathCtx, struct lws_http_mount *mount, unsigned int *idx)
{
    const char *xpathLangLoc = "/configuration/language-resources/@location";
    const char *xpathLangs = "/configuration/language-resources/languages";
    const char *xpathLangVer = "/configuration/language-resources/languages[%d]/version/@str";
    const char *xpathLang = "/configuration/language-resources/languages[%d]/language";
    const char *xpathLangCode = "/configuration/language-resources/languages[%d]/language[%d]/text()";
    char buffer[NWS_MAX_PATH_LENGTH];
    NwsErrors result = NwsError_Ok;
    char *langLoc = NULL;
    char *langVer = NULL;
    char *langCode = NULL;
    char *tmp = NULL;
    unsigned int lanVerCnt = 0;
    unsigned int lanCnt = 0;
    unsigned int expectedSize;
    unsigned int verIdx;
    unsigned int lanIdx;

    if(nswFindMatchedCount(ctx, xpathCtx, xpathLangs, &lanVerCnt) != NwsError_Ok) {
        return result;
    }

    if(lanVerCnt == 0) {
        SYSERR(SYS_INFO, "No other pages is found!");
        /* missing resources is ok, this means the website does not need other resources */
        return result;
    }

    if(lanVerCnt > NWS_MAX_LANG_VER) {
        SYSERR(SYS_INFO, "Number of language versions is more than the maximum allowed of %d pages", NWS_MAX_LANG_VER);
        result = NwsError_ConfigurationFile;
        return result;
    }
    /* get the language resource location */
    if((result = nswFindElementGetText(ctx, xpathCtx, xpathLangLoc, &langLoc)) != NwsError_Ok) {
        return result;
    }

    for(verIdx = 1; verIdx <= lanVerCnt; verIdx++) {
        lanCnt = 0;
        /* determine how many translation this language version has */
        snprintf(buffer, NWS_MAX_PATH_LENGTH, xpathLang, verIdx);
        if(    nswFindMatchedCount(ctx, xpathCtx, buffer, &lanCnt) != NwsError_Ok
            || lanCnt == 0) {
            continue;
        }
        /* get the version number */
        snprintf(buffer, NWS_MAX_PATH_LENGTH, xpathLangVer, verIdx);
        if((result = nswFindElementGetText(ctx, xpathCtx, buffer, &langVer)) != NwsError_Ok) {
            break;
        }
        for(lanIdx = 1; lanIdx <= lanCnt; lanIdx++) {
            if(*idx >= NWS_MAX_PAGES) {
                SYSERR(SYS_INFO, "Number of resources and pages is more than allowed of %d pages", NWS_MAX_PAGES);
                result = NwsError_ConfigurationFile;
                break;
            }
            /* get the language code */
            snprintf(buffer, NWS_MAX_PATH_LENGTH, xpathLangCode, verIdx, lanIdx);
            if((result=nswFindElementGetText(ctx, xpathCtx, buffer, &langCode)) != NwsError_Ok) {
                break;
            }

            expectedSize = snprintf(0, 0, "/language/%s/%s", langVer, langCode) + 1;
            tmp = NWSALLOC(ctx, expectedSize);
            snprintf(tmp, expectedSize, "/language/%s/%s", langVer, langCode);
            mount[*idx].mountpoint = tmp;
            mount[*idx].mountpoint_len = (unsigned char)strlen(tmp);
            mount[*idx].origin_protocol = LWSMPRO_FILE;

            expectedSize = snprintf(0, 0, "%s.ftl", langCode) + 1;
            tmp = NWSALLOC(ctx, expectedSize);
            snprintf(tmp, expectedSize, "%s.ftl", langCode);
            mount[*idx].def = tmp;

            expectedSize = snprintf(0, 0, "%s/%s/%s", ctx->mounts.html, langLoc, langVer) + 1;
            tmp = NWSALLOC(ctx, expectedSize);
            snprintf(tmp, expectedSize, "%s/%s/%s", ctx->mounts.html, langLoc, langVer);
            mount[*idx].origin = tmp;

            mount[*idx].extra_mimetypes = &nwsFtlMime;

            NWSFREE(ctx, langCode, strbuflen(langCode));

            *idx = *idx + 1;
        }
        NWSFREE(ctx, langVer, strbuflen(langVer));
        if(result != NwsError_Ok) {
            break;
        }
    }
    NWSFREE(ctx, langLoc, strbuflen(langLoc));

    return result;
}
/** @ingroup group_internal
  * @brief Get the pages from the configuration file
  *
  * This function parse the webpage pages from the configuration file
  *
  * @param[in] ctx Webserver context
  * @param[in] xpathCtx Configuration file where to search the webpages
  * @param[in] mount Pointer to the mount structure where to add the pages
  * @param[in, out] idx index of mount, this is populated on return with the final index used
  * @return NwsError_Ok if no error occured
  * @return NwsError_ConfigurationFile Failed to parse the configuration file. Check the log for details
  * @return NwsError_Ng General error
  */
static NwsErrors nwsParsePages(NwsContextInternal *ctx, xmlXPathContextPtr xpathCtx, struct lws_http_mount *mount, unsigned int *idx)
{
    const char *xpathResource = "/configuration/website/resource";
    const char *xpathResourceUrl = "/configuration/website/resource[%d]/@url";
    const char *xpathResourceSrc = "/configuration/website/resource[%d]/@source";
    char buffer[NWS_MAX_PATH_LENGTH];
    unsigned int count = 0;
    unsigned int index;
    NwsErrors result = NwsError_Ok;
    char *tmpUrl;
    char *tmpSrc;
    char *tmp;
    char *pos;

    if(nswFindMatchedCount(ctx, xpathCtx, xpathResource, &count) != NwsError_Ok) {
        return result;
    }
    if(count == 0) {
        SYSERR(SYS_INFO, "No other pages is found!");
        /* missing resources is ok, this means the website does not need other resources */
        return result;
    }
    if(count > NWS_MAX_PAGES) {
        SYSERR(SYS_INFO, "Number of webpages more than the maximum allowed of %d pages", NWS_MAX_PAGES);
        result = NwsError_ConfigurationFile;
        return result;
    }

    /* get the number of resource element in the configuration file */
    for(index = 1; index <= count; index++) {
        tmpUrl = NULL;
        tmpSrc = NULL;

        if(*idx >= NWS_MAX_PAGES) {
            SYSERR(SYS_INFO, "Number of resources and pages is more than allowed of %d pages", NWS_MAX_PAGES);
            result = NwsError_ConfigurationFile;
            break;
        }
        /* get url */
        snprintf(buffer, sizeof(buffer), xpathResourceUrl, index);
        if((result = nswFindElementGetText(ctx, xpathCtx, buffer, &tmpUrl)) != NwsError_Ok) {
            break;
        }
        /* get location */
        snprintf(buffer, sizeof(buffer), xpathResourceSrc, index);
        if((result = nswFindElementGetText(ctx, xpathCtx, buffer, &tmpSrc)) != NwsError_Ok) {
            NWSFREE(ctx, tmpUrl, strbuflen(tmpUrl));
            break;
        }
        memset(&mount[*idx], 0, sizeof(struct lws_http_mount));
        mount[*idx].mountpoint = tmpUrl;
        mount[*idx].mountpoint_len = (unsigned char)strlen(tmpUrl);
        mount[*idx].origin_protocol = LWSMPRO_FILE;

        /* get the relative path and filename of the resource */
        pos = strrchr(tmpSrc,'/');
        if(    pos == NULL
            || pos == tmpSrc) { // not found, meaning there is only the resource name
            mount[*idx].def = tmpSrc;
            tmp = (char*)NWSALLOC(ctx, strbuflen(ctx->mounts.html));
            memset(tmp, 0, strbuflen(ctx->mounts.html));
            strncpy(tmp, ctx->mounts.html, strlen(ctx->mounts.html));
            mount[*idx].origin = tmp;
        } else if((unsigned int)(pos - tmpSrc) == (unsigned int)strlen(tmpSrc) - 1){ // no file name
            SYSERR(SYS_INFO, "Resource source value \"%s\" is invalid", tmpSrc);
            NWSFREE(ctx, tmpUrl, strbuflen(tmpUrl));
            NWSFREE(ctx, tmpSrc, strbuflen(tmpSrc));
            result = NwsError_ConfigurationFile;
            break;
        } else {
            *pos = 0;
            char *rcPath = tmpSrc;
            char *rcName = pos + 1;
            unsigned int size;
            tmp = (char*)NWSALLOC(ctx, strbuflen(rcName));
            memset(tmp, 0, strbuflen(rcName));
            strncpy(tmp, rcName, strlen(rcName));
            mount[*idx].def = tmp;

            size = snprintf(0, 0, "%s/%s", ctx->mounts.html, rcPath) + 1;
            tmp = (char*)NWSALLOC(ctx, size);
            snprintf(tmp, size, "%s/%s", ctx->mounts.html, rcPath);
            mount[*idx].origin = tmp;
            NWSFREE(ctx, tmpSrc, strbuflen(rcPath) + strbuflen(rcName));
        }

        *idx = *idx + 1;
    }

    return result;
}

/** @ingroup group_internal
  * @brief Get the default page from the configuration file
  *
  * This function parse the default webpage from the configuration file
  *
  * @param[in] ctx Webserver context
  * @param[in] mount Pointer to the mount structre where to add the default page
  * @param[in] xpathCtx Configuration file where to search the default
  * @return NwsError_Ok if no error occured
  * @return NwsError_ConfigurationFile Failed to parse the configuration file. Check the log for details
  * @return NwsError_Ng General error
  */
static NwsErrors nwsParseDefaultPage(NwsContextInternal *ctx, struct lws_http_mount *mount, xmlXPathContextPtr xpathCtx)
{
    const char *xpath = "/configuration/website/default/text()";
    NwsErrors result = NwsError_Ok;
    char *tmp = NULL;

    if((result = nswFindElementGetText(ctx, xpathCtx, xpath, &tmp)) != NwsError_Ok) {
        return result;
    }
    memset(mount, 0, sizeof(struct lws_http_mount));
    mount->def = tmp;
    mount->mount_next = NULL;
    mount->origin_protocol = LWSMPRO_FILE;
    tmp = (char*)NWSALLOC(ctx, 2);
    tmp[0] = '/';
    tmp[1] = 0;
    mount->mountpoint = tmp;
    mount->mountpoint_len = 1;

    tmp = (char*)NWSALLOC(ctx, strbuflen(ctx->mounts.html));
    memset(tmp, 0, strbuflen(ctx->mounts.html));
    strncpy(tmp, ctx->mounts.html, strlen(ctx->mounts.html));
    mount->origin = tmp;

    return result;
}
/** @ingroup group_internal
  * @brief Get the default page from the configuration file
  *
  * This function parse the default webpage from the configuration file
  *
  * @param[in] ctx Webserver context
  * @param[in] xpathCtx Configuration file where to search the webpages
  * @param[in] cfg Configuration buffer
  * @param[in] mount Pointer to the mount structure where to add the pages
  * @param[in, out] idx index of mount, this is populated on return with the final index used
  * @return NwsError_Ok if no error occured
  * @return NwsError_ConfigurationFile Failed to parse the configuration file. Check the log for details
  * @return NwsError_Ng General error
  */
static NwsErrors nwsParseRedirects(NwsContextInternal *ctx, xmlXPathContextPtr xpathCtx,
    NwsConfiguration *cfg, struct lws_http_mount *mount, unsigned int *idx)
{
    NwsCfgRedirects *redirects;
    const char *xpathLocation= "/configuration/redirects/@location";
    const char *xpathRedirect= "/configuration/redirects/redirect";
    const char *xpathCode= "/configuration/redirects/redirect[%d]/@code";
    const char *xpathName= "/configuration/redirects/redirect[%d]/@name";
    char buffer[NWS_MAX_PATH_LENGTH];
    unsigned int count = 0;
    NwsErrors result = NwsError_Ok;
    unsigned int  redirectCode = 0;
    char *redirectLoc = NULL;
    char *redirectName = NULL;
    char *tmp;
    unsigned int length = 0;
    unsigned int index;

    if(nswFindMatchedCount(ctx, xpathCtx, xpathRedirect, &count) != NwsError_Ok) {
        return result;
    }
    if(count == 0) {
        SYSERR(SYS_INFO, "No redirect found!");
        /* missing resources is ok, this means the website does not need other resources */
        return result;
    }
    if(count > NWS_MAX_REDIRECTS) {
        SYSERR(SYS_INFO, "Number of redirects is more than allowed of %d redirects", NWS_MAX_REDIRECTS);
        result = NwsError_ConfigurationFile;
        return result;
    }

    /* get the location */
    if((result = nswFindElementGetText(ctx, xpathCtx, xpathLocation, &redirectLoc)) != NwsError_Ok) {
        return result;
    }
    redirects = &(cfg->redirects);

    memset(redirects, 0, sizeof(NwsCfgRedirects));

    redirects->cnt = count;

    /* get the number of resource element in the configuration file */
    for(index = 1; index <= count; index++) {
        if(*idx >= NWS_MAX_PAGES) {
            SYSERR(SYS_INFO, "Number of resources and pages is more than allowed of %d pages", NWS_MAX_PAGES);
            result = NwsError_ConfigurationFile;
            break;
        }
        /* get code */
        snprintf(buffer, sizeof(buffer), xpathCode, index);
        if((result = nswFindElementGetNumber(ctx, xpathCtx, buffer, &redirectCode)) != NwsError_Ok) {
            return result;
        }
        snprintf(buffer, sizeof(buffer), xpathName, index);
        if((result = nswFindElementGetText(ctx, xpathCtx, buffer, &redirectName)) != NwsError_Ok) {
            return result;
        }

        /* set redirect mount information */
        memset(&(mount[*idx]), 0, sizeof(struct lws_http_mount));

        mount[*idx].def = redirectName;
        mount[*idx].origin_protocol = LWSMPRO_FILE;

        length = snprintf(0, 0, "%s/%s", redirectLoc, redirectName) + 1;
        tmp = NWSALLOC(ctx, length);
        snprintf(tmp, length, "%s/%s", redirectLoc, redirectName);
        mount[*idx].mountpoint = tmp;

        length = snprintf(0, 0, "%s/%s", ctx->mounts.html, redirectLoc) + 1;
        tmp = NWSALLOC(ctx, length);
        snprintf(tmp, length, "%s/%s", ctx->mounts.html, redirectLoc);
        mount[*idx].origin = tmp;

        /* set redirects information */
        NwsCfgRedirect *redirect = &(redirects->redirect[index-1]);

        redirect->code = redirectCode;
        snprintf(redirect->page, NWS_MAX_NAME_LENGTH, "%s/%s", redirectLoc, redirectName);

        *idx= *idx + 1;
    }

    NWSFREE(ctx, redirectLoc, strbuflen(redirectLoc));

    return result;
}
/** @ingroup group_internal
  * @brief call to parse the specified configuration file
  *
  * Parse the specified configuration file, the configuration file format is checked before loading the file.
  * Source of the file (signatuire) is no longer verified because this function assumed such verification is
  * already performed before calling this function
  *
  * @param[in] pkq Path of the package where to find the configuration file
  * @param[in] ctx Webserver context, will receive the information about the parsed config file
  * @return NwsError_Ok if no error occured
  * @return NwsError_ConfigurationFile Failed to load the configuration file. Check the log for details
  * @return NwsError_NoConfigurationFile Specified configuration file does not exists.
  * @return NwsError_Ng General error
  */
NwsErrors nwsParseConfig(const char* pkg, NwsContextInternal *ctx)
{
    unsigned int idx = 1; //index must start in 1 as index zero is reserved for default page
    char cfgPath[NWS_MAX_PATH_LENGTH];
    struct stat fileStat;
    NwsConfiguration cfg;
    xmlDoc *doc = NULL;
    xmlXPathContextPtr xpathCtx;
    NwsErrors result = NwsError_Ok;
    unsigned int index;

    if(    pkg == NULL
        || ctx == NULL) {
        SYSERR(SYS_INFO, "Invalid parameters");
        return NwsError_Ng;
    }

    snprintf(cfgPath, sizeof(cfgPath), "%s/configuration.xml", pkg);
    /* make sure location and config file exists */
    if (stat(pkg, &fileStat) == -1) {
        SYSERR(SYS_INFO, "Mount directory (%s) does not exists", pkg);
        return NwsError_NoWebsitePackage;
    }

    if (stat(cfgPath, &fileStat) == -1) {
        SYSERR(SYS_INFO, "Configuration file (%s) not found", cfgPath);
        return NwsError_NoConfigurationFile;
    }
    /* clear the configuration buffer, exclude the version as it is set before the call to this function */
    memset(&cfg, 0, sizeof(NwsConfiguration));

    cfg.ver.maj = ctx->cfg.ver.maj;
    cfg.ver.min = ctx->cfg.ver.min;

    if ((doc = xmlReadFile(cfgPath, NULL, 0)) == NULL) {
        SYSERR(SYS_INFO, "Unable to load %s file. Maybe a non-well formed xml file.", cfgPath);
        return NwsError_ConfigurationFile;
    }
    do {
        if((xpathCtx = xmlXPathNewContext(doc)) == NULL) {
            SYSERR(SYS_INFO, "Unable to create XPATH context for the xml configuration file");
            result = NwsError_ConfigurationFile;
            break;
        }
        /* TODO: get all the redirects */
        /* set the default page */
        if((result = nwsParseDefaultPage(ctx, cfg.pages, xpathCtx)) != NwsError_Ok) {
            break;
        }
        /* get all the language resources */
        else if((result = nwsParseLangs(ctx, xpathCtx, cfg.pages, &idx)) != NwsError_Ok) {
            break;
        }
        /* get all the pages */
        else if((result = nwsParsePages(ctx, xpathCtx, cfg.pages, &idx)) != NwsError_Ok) {
            break;
        }
        /* get all redirects */
        else if((result = nwsParseRedirects(ctx, xpathCtx, &cfg, cfg.pages, &idx)) != NwsError_Ok) {
            break;
        }

        //static NwsErrors nwsParseRedirects(NwsContextInternal *ctx, xmlXPathContextPtr xpathCtx, NwsCfgRedirects *redirects)
        xmlXPathFreeContext(xpathCtx);
    } while(0);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    if(result == NwsError_Ok) {
        memcpy(&(ctx->cfg), &(cfg), sizeof(NwsConfiguration));
        /* link all mounts */
        for(index = 1; index < idx; index++) {
            if(ctx->cfg.pages[index].mountpoint == NULL) {
                break;
            }
            ctx->cfg.pages[index-1].mount_next = &(ctx->cfg.pages[index]);
        }
    } else { /* free any resources that already allocated */
        for(index = 0; index < NWS_MAX_PAGES; index++) {
            if(cfg.pages[index].def) {
                NWSFREE(ctx, cfg.pages[index].def, strbuflen(cfg.pages[index].def));
            }
            if(cfg.pages[index].mountpoint) {
                NWSFREE(ctx, cfg.pages[index].mountpoint, strbuflen(cfg.pages[index].mountpoint));
            }
            if(cfg.pages[index].origin) {
                NWSFREE(ctx, cfg.pages[index].origin, strbuflen(cfg.pages[index].origin));
            }
        }
        idx = 0;
    }
    return result;
}
/** @ingroup group_internal
  * @brief call to verify the content of the package
  *
  * Call this function to verify the content of the website package before deploying to the mount point.
  * This function will validate the configuration file format and verify any reference file exists in the
  * specified package.
  *
  * @param[in] package Location where to find the package to verify
  * @return NwsError_Ok Package is valid
  * @return NwsError_NoConfigurationFile Specified configuration file does not exists.
  * @return NwsError_NoConfigurationFile No configuration file is found.
  * @return NwsError_ConfigurationFile Configuration file in the package is invalid.
  * @return NwsError_OneOrMoreFileMissing One or more referenced file in the config is not found in the package
  */
NwsErrors nwsConfirmPackageFiles(const char* package)
{
    PARAM_UNUSED(package);
    return NwsError_Ok;
}
/** @ingroup group_internal
  * @brief Deploy the package to the webserver mount point
  *
  * Call this function to deploy a package to the webserver mount point. Note that, user needs to verify the package
  * before calling this function. See other functions of this API group.
  *
  * @param[in] package Location where to find the package to deploy
  * @return NwsError_Ok Package is deployed successfully
  * @return NwsError_Ng An error occured while deploying the package. See logs for details.
  */
NwsErrors nwsDeployPackage(const char* package)
{
    PARAM_UNUSED(package);
    return NwsError_Ok;
}

/** @ingroup group_internal
  * @brief Call to confirm if the specified package is supported.
  *
  * Get version and checked if the specified package is supported. Note that Webserver
  * library supports all previous versions.
  *
  * @param[in] pkg Location where to check the version.
  * @param[in] ctx Webserver context, will receive the information about the parsed config file
  * @return NwsError_Ok Package is supported
  * @return NwsError_WebsitePackageVersion Package is not supported.
  * @return NwsError_ConfigurationFile Invalid configuration file
  * @return NwsError_NoConfigurationFile No configuration package is find
  * @return NwsError_Ng General error
  */
NwsErrors nwsPackageConfirmVer(const char* pkg, NwsContextInternal *ctx)
{
    char cfg[NWS_MAX_PATH_LENGTH];
    const char *majXpath = "/configuration/version/major/text()";
    const char *minXpath = "/configuration/version/minor/text()";
    struct stat fileStat;
    xmlDoc *doc = NULL;
    xmlXPathContextPtr xpathCtx;
    NwsErrors result = NwsError_Ok;
    unsigned int maj, min;

    LIBXML_TEST_VERSION

    if(    pkg == NULL
        || ctx == NULL) {
        SYSERR(SYS_INFO, "Invalid parameters");
        return NwsError_Ng;
    }

    snprintf(cfg, sizeof(cfg), "%s/configuration.xml", pkg);
    /* make sure directory and location exist */
    if (stat(pkg, &fileStat) == -1) {
        SYSERR(SYS_INFO, "Mount directory (%s) does not exists", pkg);
        return NwsError_NoWebsitePackage;
    }

    if (stat(cfg, &fileStat) == -1) {
        SYSERR(SYS_INFO, "Configuration file (%s) not found", cfg);
        return NwsError_NoConfigurationFile;
    }
    /* TODO: add XSD file */
    if ((doc = xmlReadFile(cfg, NULL, 0)) == NULL) {
        SYSERR(SYS_INFO, "Unable to load %s file. Maybe a non-well formed xml file.", cfg);
        return NwsError_ConfigurationFile;
    }

    do {
        if((xpathCtx = xmlXPathNewContext(doc)) == NULL) {
            SYSERR(SYS_INFO, "Unable to create XPATH context for the xml configuration file");
            result = NwsError_ConfigurationFile;
            break;
        }

        do {
            /* get the configuration major version number */
            if((result = nswFindElementGetNumber(ctx, xpathCtx, majXpath, &maj)) != NwsError_Ok) {
                break;
            }

            /* get the configuration major minor number */
            if((result = nswFindElementGetNumber(ctx, xpathCtx, minXpath, &min)) != NwsError_Ok) {
                break;
            }
        } while(0);

        xmlXPathFreeContext(xpathCtx);
    } while(0);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    if(result == NwsError_Ok) {
        memset(&(ctx->cfg.ver), 0, sizeof(NwsCfgVer));
        ctx->cfg.ver.maj = maj;
        ctx->cfg.ver.min = min;
        /* configuration version must be equal or less than the current webserver version */
        if(    maj > NWS_VERSION_MAJOR
            || (    maj == NWS_VERSION_MAJOR
                 && min > NWS_VERSION_MINOR)) {
            SYSERR(SYS_INFO, "Configuration v%d.%d not supported.", maj, min);
            result = NwsError_WebsitePackageVersion;
        }
    }
    return result;
}
