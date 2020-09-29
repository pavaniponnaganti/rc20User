/** @file noja_webserver.h
 * Main header file for the implementation of REL20 Webserver Library
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
#ifndef _NOJA_WEBSERVER_H
#define _NOJA_WEBSERVER_H

#include "noja_webserver_type.h"
#include "noja_webserver_cfg.h"
#include "noja_webserver_ver.h"

#if defined(QT_NWS_LIBRARY_SHARED)
    #include <QtCore/qglobal.h>
    #if defined(QT_NWS_LIBRARY)
        /* as per QT doc, this must be added to the declarations of symbols used when compiling a shared library. */
        #define QT_NWS_LIBRARY_EXPORT Q_DECL_EXPORT
    #else
        /* as per QT doc, this must be added to the declarations of symbols used when compiling a client that uses the shared library. */
        #define QT_NWS_LIBRARY_EXPORT Q_DECL_IMPORT
    #endif
#else
    #define QT_NWS_LIBRARY_EXPORT
#endif

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
QT_NWS_LIBRARY_EXPORT NwsErrors nwsInitialise(NwsHttpPort port, NwsTimeIntervalMS serviceInterval, NwsTimeIntervalS pingPongInterval,
  const char* mount, NwsContext *context);
/** @ingroup group_main_apis
  * @brief Call to install new website package and configuration.
  *
  * This API will install new configuration and website package from the path specified in the parameter. This function will confirm the
  * configuration in the path specified follows the current configuraiton schema. The function will also confirm if the current website
  * package version is supported. Following these checks, this functions will check if the website package is complete based from the
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
QT_NWS_LIBRARY_EXPORT NwsErrors nwsInstallWebsite(const char* website);
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
QT_NWS_LIBRARY_EXPORT NwsErrors nwsProcess(NwsContext ctx);
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
QT_NWS_LIBRARY_EXPORT NwsErrors nwsStop(NwsContext *ctx);

#endif
