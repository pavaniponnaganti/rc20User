/** @file noja_webserver.c
 * REL-20 Webserver interface implementation
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
#include <stdio.h>
#include "noja_webserver_cmn.h"
#include "noja_webserver_pkg.h"
#include "noja_webserver_http.h"
#include "noja_webserver_mem.h"

/** @ingroup group_main_apis
  * @brief Call to initialise the Webserver module.
  *
  * This API will initialise the Webserver module. When called, the Webserver will parse the configuration file
  * present in the mount directory. On successful configuration parsing, the Webserver will build the website based
  * from the configuration (e.g. setup all URLs, redirects). Calling this function while the Webserver is active will
  * result to error and may terminate the Webserver process.
  *
  * @param[in] port Specify the port the HTTP protocol will use. Normal value for this parameter are 80, 443 (preferred), and 8080
  * Make sure the port specified in not reserved.
  * @param[in] serviceInterval Specify the webserver process service interval. This is the minimum amount of time (in millisecond)
  * the Webserver process will be blocked to allow the processes with equal or lower priority CPU time.
  * @param[in] pingPongInterval Specify the amount of time (in seconds) the Webserver will wait for the pong reply
  * to its ping request before terminating the connection. For details about the ping-pong frames, refer to https://tools.ietf.org/html/rfc6455.
  * @param[in] mount Specify the path to the mount directory
  * @param[out] context Handle to the context of the current webserver intance.
  * @return NwsError_MountNotExists the specified mount directory does not exists
  * @return NwsError_NoWebsitePackage No website package is installed in the mount directory
  * @return NwsError_WebsitePackageVersion The current website package is not supported
  * @return NwsError_ConfigurationFile The configuration file failed to load due to error
  * @return NwsError_NoConfigurationFile Specified configuration file does not exists.
  * @return NwsError_OneOrMoreFileMissing One or more referenced file in the config is not found in the package
  * @return NwsError_Port The specified port is invalid or reserved
  * @return NwsError_SSLCertificate Unable to get the SSL certificate and private key for establishing secured connection
  * @return NwsError_Ng General error occured
  */
NwsErrors nwsInitialise(NwsHttpPort port, NwsTimeIntervalMS serviceInterval, NwsTimeIntervalS pingPongInterval, const char* mount,
    NwsContext *context)
{
    struct lws_context_creation_info createInfo;
    NwsContextInternal *ctxInt;
    NwsErrors result = NwsError_Ok;
    int index;

    if (    mount == NULL
         || context == NULL) {
        SYSERR(SYS_INFO, "Invalid parameters");
        return NwsError_Ng;
    }

    ctxInt = (NwsContextInternal*) NWSALLOC(NULL, sizeof(NwsContextInternal));
    memset(ctxInt, 0, sizeof(NwsContextInternal));
    *context = ctxInt;

    /* set website path */
    snprintf(ctxInt->mounts.html, NWS_MAX_PATH_LENGTH, "%s/%s", mount, NWS_PUBLIC_HTML);
    snprintf(ctxInt->mounts.builtins, NWS_MAX_PATH_LENGTH, "%s/%s", mount, NWS_PUBLIC_BUILTIN);
    snprintf(ctxInt->mounts.staging, NWS_MAX_PATH_LENGTH, "%s/%s", mount, NWS_MOUNT_STAGING);

    ctxInt->port = port;
    ctxInt->srvcInt = serviceInterval;
    ctxInt->ppInt = pingPongInterval;

    /* TODO: check if an existing webserver is already initialised */

    if(    (result = nwsPackageConfirmVer(ctxInt->mounts.html, ctxInt)) != NwsError_Ok
        || (result = nwsConfirmPackageFiles(ctxInt->mounts.html)) != NwsError_Ok
        || (result = nwsParseConfig(ctxInt->mounts.html, ctxInt)) != NwsError_Ok) {
        return result;
    }
    memset(&createInfo, 0, sizeof(struct lws_context_creation_info));

    createInfo.port = port;
    createInfo.mounts = ctxInt->cfg.pages;
    createInfo.ws_ping_pong_interval = pingPongInterval;
    createInfo.options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;
    createInfo.timeout_secs = pingPongInterval;
    createInfo.vhost_name = NWS_WEBSERVER_ROOT_URL;

    for(index = 0; index <= NWS_MAX_REDIRECTS; index++) {
        if(ctxInt->cfg.redirects.redirect[index].code == 404) {
            createInfo.error_document_404 = ctxInt->cfg.redirects.redirect[index].page;
            break;
        }
    }

    ctxInt->cntx = lws_create_context(&createInfo);
    if (ctxInt->cntx == NULL) {
        SYSERR(SYS_INFO, "Failed to create the websocket context");
        result = NwsError_Ng;
    } else if (lws_create_vhost(ctxInt->cntx, &createInfo) == 0) {
        SYSERR(SYS_INFO, "Failed to create the vhost");
        result = NwsError_Ng;
    }
#if defined(NWS_DEBUG)
    NwsConfiguration *cfg = &(ctxInt->cfg);
    fprintf(stdout, "------ Configuration ------\n");
    fprintf(stdout, "Version: %d.%d\n", cfg->ver.maj, cfg->ver.min);
    fprintf(stdout, "Website:\n");
    for(index = 0; index < NWS_MAX_PAGES; index++) {
        struct lws_http_mount *curPage = &(cfg->pages[index]);
        if(curPage->mountpoint == NULL) {
            continue;
        }
        while(strchr(curPage->origin,'\\')) {
            char *tmp = strchr(curPage->origin,'\\');
            *tmp = '/';
        }
        while(strchr(curPage->mountpoint,'\\')) {
            char *tmp = strchr(curPage->mountpoint,'\\');
            *tmp = '/';
        }
        fprintf(stdout, "    [%d] Address: %08Xh; Next: %08Xh\n", index, (unsigned int)curPage, (unsigned int)curPage->mount_next);
        fprintf(stdout, "         Origin: %s\n", curPage->origin ? curPage->origin : "NULL");
        fprintf(stdout, "         Protocol: %08Xh\n", (unsigned int)curPage->origin_protocol);
        fprintf(stdout, "         Mount Point: %s\n", curPage->mountpoint ? curPage->mountpoint : "NULL");
        fprintf(stdout, "         Mount Point Length: %d\n", (unsigned int)curPage->mountpoint_len);
        fprintf(stdout, "         Def: %s\n", curPage->def ? curPage->def : "NULL");
    }
    NwsCfgRedirects *redirects = &(cfg->redirects);
    fprintf(stdout, "Redirects:\n");
    for(index = 0; index < redirects->cnt; index++) {
        NwsCfgRedirect *redirect = &(redirects->redirect[index]);
        fprintf(stdout, "    [%d] Code: %d\n", index, redirect->code);
        fprintf(stdout, "         URL: %s\n",redirect->page);
    }
#endif
    return result;
}
/** @ingroup group_main_apis
  * @brief Call to install new website package and configuration.
  *
  * This API will install new configuration and website package from the path specified in the parameter. This function will confirm the
  * configuration in the path specified follows the current configuraiton schema. The function will also confirm if the current website
  * package version is supported. Following these checks, this functions will check of the website package is complete based from the
  * configuration file included.
  *
  * @param[in] website Specify the path of the website package to install.
  * @return NwsError_Ok if no error occured
  * @return NwsError_NoWebsitePackage No website package to install or the website package is invalid
  * @return NwsError_WebsitePackageVersion The current website package is not supported
  * @return NwsError_WebsitePackageSignature Website package failed the signature verification and can't be trusted
  * @return NwsError_ConfigurationFile The configuration file failed to load due to error
  * @return NwsError_Ng General error occured
  */
NwsErrors nwsInstallWebsite(const char* website)
{
    PARAM_UNUSED(website);

    return NwsError_Ok;
}
/** @ingroup group_main_apis
  * @brief Call to allow the Webserver to process and reply to requests
  *
  * Webserver process should call this function to allow the Webserver to handle new and pending requests. This function should
  * be called after a successful nwsInitialise call. Webserver will become active on initial call to this function. Call to this
  * function is blocking. If this function failed, the process should call the nwsStop function, calling this function again after
  * the previous call failed may result to unexpected behaviour.
  *
  * @param[in] ctx Context handle for the webserver to process.
  * @return NwsError_Ok if no error occured
  * @return NwsError_WebserverActive Another instance of the webserver is already running
  * @return NwsError_WebserverNotInitialise Webserver is currently not initialised
  * @return NwsError_Ng error occurred while the webserver is processing the requests
  */
NwsErrors nwsProcess(NwsContext ctx)
{
    int result = 0;
    NwsContextInternal *ctxInt = ctx;
    if (ctx == NULL) {
        return NwsError_Ng;
    }

    if((result =  lws_service(ctxInt->cntx, ctxInt->srvcInt)) < 0) {
        SYSERR(SYS_INFO, "Webserver lws_service failed! Error code: %d", result);
        return NwsError_Ng;
    }
    return NwsError_Ok;
}
/** @ingroup group_main_apis
  * @brief Call to terminate the Webserver service
  *
  * This API will stop the Webserver service. All current webserver sessions will terminate and may result to HTTP status 500 or 503
  * in the client browser. When Webserver is stopped, the Webserver needs to intialise again before calling the nwsProcess function.
  *
  * @param[in] ctx Pointer to the context handle for the webserver to stop.
  * @return NwsError_Ok if no error occured
  * @return NwsError_WebserverNotActive There is no webserver to stop
  * @return NwsError_Ng error occurred while trying to stop the webserver
  */
NwsErrors nwsStop(NwsContext *ctx)
{
    NwsContextInternal *contextLoc = NULL;
    int index;

    if (    ctx == NULL
         || *ctx == NULL) {
        SYSERR(SYS_INFO, "Invalid parameter");
        return NwsError_Ng;
    }

    contextLoc = (NwsContextInternal *) *ctx;

    if(contextLoc->cntx) {
        lws_context_destroy(contextLoc->cntx);
    }

    /* release resources from mount point */
    for(index = 0; index < NWS_MAX_PAGES; index++) {
        if(contextLoc->cfg.pages[index].def) {
            NWSFREE(contextLoc, (void*)contextLoc->cfg.pages[index].def, strbuflen(contextLoc->cfg.pages[index].def));
        }
        if(contextLoc->cfg.pages[index].mountpoint) {
            NWSFREE(contextLoc, (void*)contextLoc->cfg.pages[index].mountpoint, strbuflen(contextLoc->cfg.pages[index].mountpoint));
        }
        if(contextLoc->cfg.pages[index].origin) {
            NWSFREE(contextLoc, (void*)contextLoc->cfg.pages[index].origin, strbuflen(contextLoc->cfg.pages[index].origin));
        }
    }
    if(contextLoc->memAllocCnt || contextLoc->memAllocSize) {
        SYSERR(SYS_INFO,
            "Possible memory leak exists!. Max Memory Allocated=%d bytes; Remaining Allocated"
            " Count: %d; Remaining Allocated Size: %d bytes", contextLoc->memHighSize + sizeof(NwsContextInternal),
            contextLoc->memAllocCnt, contextLoc->memAllocSize);
    }

    NWSFREE(NULL, contextLoc, sizeof(NwsContextInternal));

    *ctx = NULL;

    return NwsError_Ok;
}
