/** @file noja_webserver_cfg.h
 * Contain common libwebserver configurations declaration
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
#ifndef _NOJA_WEBSERVER_CFG_H
#define _NOJA_WEBSERVER_CFG_H

/** HTTPS default port number */
#define NWS_PORT_DEFAULT                80
/** Websocket defaukt pingpong interval in seconds */
#define NWS_PINGPONG_INTERVAL_DEFAULT   10
/** libwebsockets defaukt service interval in milliseconds */
#define NWS_SERVICE_INTERVAL_DEFAULT    100
/** Webserver mount point */
#define NWS_MOUNT_DIR                   "/var/www/html/mount"
/** MOUNT directory relative to MOUNT directory */
#define NWS_MOUNT_PUBLIC                "public"
/** HTML directory relative to PUBLIC directory */
#define NWS_PUBLIC_HTML                 NWS_MOUNT_PUBLIC "/html"
/** built-ins directory relative to PUBLIC directory */
#define NWS_PUBLIC_BUILTIN              NWS_MOUNT_PUBLIC "/built-in"
/** built-ins directory relative to PUBLIC directory */
#define NWS_MOUNT_STAGING               "staging"
/** Maximum length of resource path */
#define NWS_MAX_PATH_LENGTH                 255
/** Maximum number of language versions */
#define NWS_MAX_LANG_VER                    2
/** Maximum language translation a language version can have */
#define NWS_MAX_LANG_TRANSLATIONS           10
/** Maximum length of any name entity in any Webserver structure */
#define NWS_MAX_NAME_LENGTH                 255
/** define the maximum pages the website can have */
#define NWS_MAX_PAGES                       100
/** define the maximum redirects the webserver can support */
#define NWS_MAX_REDIRECTS                   10
/** specify the maximum protocol and subprotocol supported */
#define NWS_MAX_PROTOCOLS                   10
/** Webserver default virtual host name */
#define NWS_WEBSERVER_ROOT_URL              "relay20xx.nojapower.com"

#endif
