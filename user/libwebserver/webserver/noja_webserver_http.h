/** @file noja_webserver_http.h
 * HTTP handling callback for webserver
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
#ifndef NOJA_WEBSERVER_HTTP_H
#define NOJA_WEBSERVER_HTTP_H

#include "libwebsockets.h"

/** @ingroup group_internal
  * @brief HTTP Webserver handling
  *
  * Handle requests comming from the HTTP protocol
  *
  * @param[in] wsi WEbsocket instance pointer
  * @param[in] reason Reason for calling the callback
  * @param[in] user Session specific user data
  * @param[in] in pointer to calback parameters
  * @param[in] len length of in buffer
  * @return libwebsockets result code
  */
extern int nwsHttpCb(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

#endif
