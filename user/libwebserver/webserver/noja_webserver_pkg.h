/** @file noja_webserver_pkg.h
 * Contains package handling headers
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
#ifndef NOJA_WEBSERVER_PKG_H
#define NOJA_WEBSERVER_PKG_H

#include "noja_webserver_cmn.h"

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
NwsErrors nwsParseConfig(const char* pkg, NwsContextInternal *ctx);

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
NwsErrors nwsConfirmPackageFiles(const char* package);

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
NwsErrors nwsDeployPackage(const char* package);

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
  * @return NwsError_NoConfigurationFile No configuration package is find
  * @return NwsError_Ng General error
  */
NwsErrors nwsPackageConfirmVer(const char* pkg, NwsContextInternal *ctx);

// * @ingroup group_internal
//   * @brief Call to load the specified configuration file into the webserver
//   *
//   * This function will load specified the into the webserver.
//   *
//   * @param[in] ctx Webserver contenxt
//   * @param[in] mount Libwebsocket mount configurations buffer
//   * @return NwsError_Ok Package is supported
//   * @return NwsError_WebsitePackageVersion Package is not supported.
//   * @return NwsError_Ng General error occured
// NwsErrors nwsPackageLoad(NwsContextInternal *ctx, struct lws_http_mount *mount);

#endif
