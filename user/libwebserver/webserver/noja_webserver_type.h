/** @file noja_webserver_type.h
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
#ifndef _NOJA_WEBSERVER_TYPE_H
#define _NOJA_WEBSERVER_TYPE_H

/** @ingroup group_main_apis
  * @brief Webserver handle to distinguish different webserver context */
typedef void* NwsContext;
/** @ingroup group_main_apis
  * @brief 16-bit HTTP port number */
typedef unsigned short NwsHttpPort;
/** @ingroup group_main_apis
  * @brief 8-bit time interval. Each tick represent one second */
typedef unsigned char NwsTimeIntervalS;

/** @ingroup group_main_apis
  * @brief 16-bit time interval. Each tick represent one millisecond */
typedef unsigned short NwsTimeIntervalMS;

/** @ingroup group_main_apis
  * @brief List of error codes for Webserver APIs
  *
  * The following enum is a list of Webserver API implemntation error codes.
  * NOTE: This enum should be defined in the Data Manager
  */
enum NwsErrors_e {
    /** No error occured */
    NwsError_Ok,
    /** WEbserver context is NULL or invalid*/
    NwsError_InvalidContext,
    /** ping-pong interval is out-of-range */
    NwsError_PingPongInterval,
    /** specified mount directory does not exists */
    NwsError_MountNotExists,
    /** no website package to load or website package has missing/invalid resources*/
    NwsError_NoWebsitePackage,
    /** website package failed signature check  */
    NwsError_WebsitePackageSignature,
    /** website package version is not supported by the current Webserver version  */
    NwsError_WebsitePackageVersion,
    /** configuration file failed to load */
    NwsError_ConfigurationFile,
    /** configuration file not found */
    NwsError_NoConfigurationFile,
    /** specified port is invalid or reserved */
    NwsError_Port,
    /** no SSL certificate and private key to use for secured connection */
    NwsError_SSLCertificate,
    /** the webserver is already active */
    NwsError_WebserverActive,
    /** the webserver is already not active */
    NwsError_WebserverNotActive,
    /** the webserver not initialise */
    NwsError_WebserverNotInitialise,
    /** One or more file in the website package is missing */
    NwsError_OneOrMoreFileMissing,
    /** General error occured */
    NwsError_Ng
};

/** @ingroup group_main_apis
  * @brief List of error codes for Webserver APIs*/
typedef enum NwsErrors_e NwsErrors;

#endif
