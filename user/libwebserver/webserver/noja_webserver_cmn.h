/** @file noja_webserver_cmn.h
 * Common header file contains incldue to common and internal declarations
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
#ifndef _NOJA_WEBSERVER_CMN_H
#define _NOJA_WEBSERVER_CMN_H

#include "libwebsockets.h"
#if !defined(QT_CORE_LIB)
#include "dbApi.h"
#endif
#include "datapointEnum.h"
#include "noja_webserver_type.h"
#include "noja_webserver_cfg.h"
#include "noja_webserver_ver.h"

#pragma pack(4)

/** @ingroup group_internal
  * @brief get string length + 1 */
#define strbuflen(x) (strlen((const char *)(x)) + 1)

/** @ingroup group_internal
  * @brief Webserver context internal def */
typedef struct NwsContextInternal_st  NwsContextInternal;

/** @ingroup group_internal
  * @brief Webserver config structure */
typedef struct NwsConfiguration_st NwsConfiguration;

/** @ingroup group_internal
  * @brief Webserver version structure */
typedef struct NwsCfgVer_st NwsCfgVer;

/** @ingroup group_internal
  * @brief Webserver languages structure */
typedef struct NwsCfgLangs_st NwsCfgLangs;

/** @ingroup group_internal
  * @brief Webserver language translation structure */
typedef struct NwsCfgLangTrans_st NwsCfgLangTrans;

/** @ingroup group_internal
  * @brief Webserver redirects page structure */
typedef struct NwsCfgRedirects_st NwsCfgRedirects;

/** @ingroup group_internal
  * @brief Webserver redirect page structure */
typedef struct NwsCfgRedirect_st NwsCfgRedirect;

/** @ingroup group_internal
  * @brief Webserver website structure */
typedef struct NwsCfgPages_st NwsCfgPages;

/** @ingroup group_internal
  * @brief Webserver page info */
typedef struct NwsCfgPage_st NwsCfgPage;

/** @ingroup group_internal
  * @brief Webserver mount structure */
typedef struct NwsMounts_st NwsMounts;


/** @ingroup group_internal
  * @brief Webserver version structure
  *
  * Define the Webserver Configuration version information
  */
struct NwsCfgVer_st {
    /** Version major number */
    unsigned char maj;
    /** Version minor number */
    unsigned char min;
};

/** @ingroup group_internal
  * @brief Webserver redirect info structure
  *
  * Define the Webserver redirect page
  */
struct NwsCfgRedirect_st {
    /* httml code to redirect */
    unsigned short code;
    /* list of language versions */
    char page[NWS_MAX_NAME_LENGTH];
};

/** @ingroup group_internal
  * @brief Webserver redirects page structure
  *
  * Define the Webserver redirect pages
  */
struct NwsCfgRedirects_st {
    /* specify the number of redirect element */
    unsigned char cnt;
    /* list of language versions */
    NwsCfgRedirect redirect[NWS_MAX_REDIRECTS];
};

/** @ingroup group_internal
  * @brief Webserver page info */
struct NwsCfgPage_st {
    /* specify the URL of the current page with respect to the root URL */
    char url[NWS_MAX_PATH_LENGTH];
    /* specify the source path of the page with respect to HTML folder */
    char source[NWS_MAX_PATH_LENGTH];
};

/** @ingroup group_internal
  * @brief Webserver config structure
  *
  * Define the Webserver Configuration
  */
struct NwsConfiguration_st {
    /** contains the version of this configuration */
    NwsCfgVer ver;
    // * contains information about the availabel strings translation for this website
    // NwsCfgLangs langs;
    /** contain the info about the pages to use when redirecting the request */
    NwsCfgRedirects redirects;
    /** contain the info about the website structure */
    struct lws_http_mount pages[NWS_MAX_PAGES];
};

/** @ingroup group_internal
  * @brief Webserver mount structure
  *
  * Define the Webserver mount folder
  */
struct NwsMounts_st {
   /** website root folder */
   char html[NWS_MAX_PATH_LENGTH];
   /** built-ins resources folder */
   char builtins[NWS_MAX_PATH_LENGTH];
   /** webserver website staging area */
   char staging[NWS_MAX_PATH_LENGTH];
};

/** @ingroup group_internal
  * @brief Webserver context internal def
  *
  * Redefinition of context type, for iternal use ronly
  */
struct NwsContextInternal_st {
    /** HTTP port */
    NwsHttpPort port;
    /* Libwebsocket service interval */
    NwsTimeIntervalMS srvcInt;
    /* Libwebsocket ping-pong interval */
    NwsTimeIntervalS ppInt;
    /** mount directory */
    NwsMounts mounts;
    /** webserver configuration */
    NwsConfiguration cfg;
    /** List of supported protocol and subprotocols */
    struct lws_protocols prot[NWS_MAX_PROTOCOLS];
    /* Libwebsockets context handle */
    struct lws_context* cntx;
    /* total memory allocation for this context */
    unsigned int memAllocCnt;
    /* total size of memory allocation for this context */
    unsigned int memAllocSize;
    /* highest size of memory allocation for this context */
    unsigned int memHighSize;
};

#pragma pack()

#if defined(QT_CORE_LIB)
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #if defined(SYS_INFO)
        #undef SYS_INFO
    #endif
    #if defined(SYSERR)
        #undef SYSERR
    #endif
    #define SYS_INFO 0
    #define SYSERR(x, ...) { fprintf(stdout, __VA_ARGS__); fflush(stdout); }
#endif

#define PARAM_UNUSED(x) ((void)(x))

#endif
