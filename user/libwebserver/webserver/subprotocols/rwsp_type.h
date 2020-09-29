/** @file rwsp_type.h
 * NOJA Relay WebSockets Protocol type declration
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
#ifndef _RWSP_TYPE_H
#define _RWSP_TYPE_H

#include "cJSON.h"

/** @ingroup group_prot_apis
  * @brief Relay WebSockets Protocol data buffer pointer */
typedef unsigned char* RwspData;

/** @ingroup group_prot_apis
  * @brief Relay WebSockets Protocol data buffer size */
typedef unsigned short RwspDataSize;

/** @ingroup group_prot_apis
  * @brief Relay WebSockets Protocol connection context */
typedef struct RwspCtx_St RwspCtx;

/** @ingroup group_main_apis
  * @brief Relay WebSockets Protocol errors */
typedef enum RwspErrors_e RwspErrors;

/** @ingroup group_main_apis
  * @brief Relay WebSockets Protocol content types */
typedef enum RwspContentTypes_e RwspContentTypes;

/** @ingroup group_prot_apis
  * @brief Relay WebSockets Protocol message reception callback function
  *
  * Callback function the Relay WebSockets Protocol will call when message is received for the subprotocol.
  *
  * @param[in] context Defines the attribute of the communication channel where the message was recieved.
  * @param[in] data Buffer to the data to send. This buffer is returned by the decode callback function it is assumed
  * the subprotocol will free this buffer if required.
  * @param[in] size Specify the size of the data pointed by data buffer.
  *
  * @return NwsError_Ok if no error occured
  * @return NwsError_Ng General error occured
  */
typedef RwspErrors (*RwspRcvdCb)(RwspCtx *context, RwspData data, RwspDataSize size);

/** @ingroup group_prot_apis
  * @brief Convert the subprotocol data into JSON compatible string data
  *
  * Relay WebSockets Protocol callback to convert subprotocol specific data into CJSON library JSON object.
  *
  * @param[in] buffer Buffer to the byte stream to convert. Callback should not free this buffer.
  * @param[in] size Specify the size of the data pointed by buffer
  * @param[out] content Converted data in CJSON library JSON object. See https://github.com/DaveGamble/cJSON for details.
  * of CJSON library. This resource will be freed by the Relay WebSockets Protocol implementation using the CJSON
  * cJSON_Delete function.
  * @param[out] contentType Specify the type of the encoded data.
  *
  * @return NwsError_Ok if no error occured
  * @return NwsError_Ng General error occured
  */
typedef RwspErrors (*RwspEncCb)(RwspData buffer, RwspDataSize size, cJSON **json);

/** @ingroup group_prot_apis
  * @brief Convert the JSON string data
  *
  * Relay WebSockets Protocol callback to CJSON Library JSON object into the subprotocol specific data format.
  *
  * @param[in] json Pointer to the cJSON library JSON object. See https://github.com/DaveGamble/cJSON for details. This callback
  * should not free this object.
  * @param[in]  contentType Specify the content type of the JSON object.
  * @param[out] buffer Buffer to the byte stream to convert. This buffer will be send to the subprotocol for handling and
  * assumed the subprotocol will handle any freeing needed for this buffer.
  * @param[out] size Specify the size of the data pointed by buffer.
  *
  * @return NwsError_Ok if no error occured
  * @return NwsError_Ng General error occured
  */
typedef RwspErrors (*RwspDecCb)(cJSON *json, RwspData *buffer, RwspDataSize *size);

/** @ingroup group_prot_apis
  * @brief Convert the JSON string data */
typedef RwspErrors (*RwspFreeCb)(RwspData buffer, RwspDataSize size);

/** @ingroup group_prot_apis
  * @brief Relay WebSockets Protocol connection context
  *
  * This structure contained the required information identifying the
  * WebSocket channel who own the request.
  */
struct RwspCtx_St {
    unsigned char dummy;
};

/** @ingroup group_prot_apis
  * @brief Relay WebSockets Protocol errors
  *
  * The following enum is a list of Relay WebSockets Protocol error codes
  */
enum RwspErrors_e {
    /** No error occured */
    RwspError_Ok,
    /** Subprotocol name already exists */
    RwspError_Name,
    /** the specified context is either invalid or the channel is already disconnected */
    RwspError_InvalidContext,
    /** The client didn't confirm the reception of the message and the Relay WebSockets
      * Protocol giveup waiting for confirmation. */
    RwspError_Timeout,
    /** No active connection is present for this subprotocol. */
    RwspError_NoConnection,
    /** General error occurred */
    RwspError_Ng
 };

 /** @ingroup group_main_apis
  * @brief Relay WebSockets Protocol content types
  *
  * Relay Websockets Protocol content-types enumeration
  */
enum RwspContentTypes_e {
  /* Content is in JSON object format (i.e application/json) */
  RwspContentType_Object,
  /* Content is in bas64 format (i.e application/octet-stream) */
  RwspContentType_OctetStream,
  /* Content is in text format (i.e plain/text) */
  RwspContentType_Text
};

#endif
