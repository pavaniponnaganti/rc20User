/** @file rwsp.c
 * Relay WebSockets Protocol implementation
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
#include "rwsp.h"
#include "noja_webserver_cmn.h"

/** @ingroup group_prot_apis
  * @brief Call to register the subprotocol to Relay WebSockets Protocol
  *
  * Call this API to register a subprotocol to the Relay WebSockets Protocol. Once registered, a WebSockets communication
  * channel can be created by a remote client to use for sending and receiving the subprotocol messages.
  *
  * @param[in] name Specify the string name of the subprotocol. This name will be use to channel the request to the correct subprotocols and
  * must be unique. Remote client will use this name to connect the Websocket to this protocol. The name must be null terminated and will not be
  * freed by the webserver after using.
  * @param[in] rcvdCb This callback is called by the Relay WebSockets Protocol to forward received message to the subprotocol. If NULL, will use the
  * default handler (see rwspRcvdCbDef function).
  * @param[in] encCb This callback is called by the Relay WebSockets Protocol to encode the subprotocol message in JSON format before sending to
  * the remote client(s). If not specified, the default handler will be use (see rwspEncCb function).
  * @param[in] decCb This callback is called by the Relay WebSockets Protocol to convert the JSON message into the subprotocol message format.
  * If not specified, the default handler will be use (see rwspDecCbDef function).
  *
  * @return RwspError_Ok if no error occured
  * @return RwspError_Name if the specified subprotocol name already exists
  * @return RwspError_Ng General error occured
  */
RwspErrors rwspReg(const char* name, RwspRcvdCb rcvdCb, RwspEncCb encCb, RwspDecCb decCb)
{
    PARAM_UNUSED(name);
    PARAM_UNUSED(rcvdCb);
    PARAM_UNUSED(encCb);
    PARAM_UNUSED(decCb);
    return RwspError_Ok;
}
/** @ingroup group_prot_apis
  * @brief Call to transmit message to the remote client.
  *
  * Transmit the message to the communication channel of the specified connection context. This function will convert the message into
  * JSON format using the RwspEncCb function before sending the data. Calling this function may not transmit the message immediately but
  * the message will be transmit as soon as possible.
  *
  * @param[in] context Defines the attribute of the communication channels the message will be sent. Relay WebSockets Protocol implementation
  * uses this to determine which connection to use to transmit the message.
  * @param[in] data Buffer to the data to send. The Relay WebSockets Protocol implementation will not free this buffer but must be available
  * as long as this function is running.
  * @param[in] size Specifyt the size of the data pointed by data buffer.
  *
  * @return RwspError_Ok if no error occured
  * @return RwspError_InvalidContext the specified context is either invalid or the channel is already disconnected
  * @return RwspError_Timeout The client didn't confirm the reception of the message and the Relay WebSockets Protocol giveup
  * waiting for confirmation.
  * @return RwspError_Ng General error occured
  */
RwspErrors rwspSnd(RwspCtx *context, RwspData data, RwspDataSize size)
{
    PARAM_UNUSED(context);
    PARAM_UNUSED(data);
    PARAM_UNUSED(size);
    return RwspError_Ok;
}
/** @ingroup group_prot_apis
  * @brief Call to transmit message to all remote clients.
  *
  * This function will transmit the message all communication channel under this subprotocol that are currently active.
  * This function will convert the message into JSON format using the RwspEncCb function before sending the data. The message may not be
  * transmitted immediately and at the same time. The broadcast message will be transmitted as soon as possible but dependent on each active
  * channel.
  *
  * @param[in] data Buffer to the data to send. The Relay WebSockets Protocol implementation will not free this buffer but must be available
  * as long as this function is running.
  * @param[in] size Specifyt the size of the data pointed by data buffer.
  *
  * @return RwspError_Ok If no error occured
  * @return RwspError_NoConnection No active connection is present for this subprotocol.
  * @return RwspError_Ng General error occured
  */
RwspErrors rwspBcast(RwspData data, RwspDataSize size)
{
    PARAM_UNUSED(data);
    PARAM_UNUSED(size);
    return RwspError_Ok;
}
/** @ingroup group_prot_apis
  * @brief Relay WebSockets Protocol message reception callback function
  *
  * Default handler for message received. This function will alwats return NwsError_Ok and  will
  * do nothing to the data received
  *
  * @param[in] context Defines the attribute of the communication channel where the message was recieved.
  * @param[in] data Buffer to the data to send. This buffer is returned by the decode callback function it is assumed
  * the subprotocol will free this buffer if required.
  * @param[in] size Specify the size of the data pointed by data buffer.
  *
  * @return NwsError_Ok if no error occured
  * @return NwsError_Ng General error occured
  */
RwspErrors rwspRcvdCbDef(RwspCtx *context, RwspData data, RwspDataSize size)
{
    PARAM_UNUSED(context);
    PARAM_UNUSED(data);
    PARAM_UNUSED(size);
    return RwspError_Ok;
}

/** @ingroup group_prot_apis
  * @brief Convert the subprotocol data into JSON compatible string data
  *
  * Default handler for converting the subprotocol specific data into JSON string. This handler will convert
  * the stream into base64 format.
  *
  * @param[in] buffer Buffer to the byte stream to convert.
  * @param[in] size Specify the size of the data pointed by buffer
  * @param[out] content Converted data in CJSON library JSON object. See https://github.com/DaveGamble/cJSON for details.
  * of CJSON library. This resource will be freed by the Relay WebSockets Protocol implementation using the CJSON
  * cJSON_Delete function.
  * @param[out] contentType Specify the type of the encoded data.
  *
  * @return NwsError_Ok if no error occured
  * @return NwsError_Ng General error occured
  */
RwspErrors rwspEncCbDef(RwspData buffer, RwspDataSize size, cJSON **content, RwspContentTypes *contentType)
{
    PARAM_UNUSED(buffer);
    PARAM_UNUSED(size);
    PARAM_UNUSED(content);
    PARAM_UNUSED(contentType);
    return RwspError_Ok;
}

/** @ingroup group_prot_apis
  * @brief Convert the JSON string data into the subprotocol format.
  *
  * Default handler for converting the JSON object into subprotocol specific data. This handler will convert
  * octet-stream into byte stream and text into null terminated string. Other content-types are not supported.
  *
  * @param[in] json Pointer to the cJSON library JSON object. See https://github.com/DaveGamble/cJSON for details. This function
  * will not free this object.
  * @param[in]  contentType Specify the content type of the JSON object.
  * @param[out] buffer Buffer to the byte stream to convert. This buffer will be send to the subprotocol for handling and
  * assumed the subprotocol will handle any freeing needed for this buffer.
  * @param[out] size Specify the size of the data pointed by buffer.
  *
  * @return RwspError_Ok if no error occured
  * @return RwspError_Ng General error occured
  */
RwspErrors rwspDecCbDef(cJSON *json, RwspContentTypes contentType, RwspData *buffer, RwspDataSize *size)
{
    PARAM_UNUSED(json);
    PARAM_UNUSED(contentType);
    PARAM_UNUSED(buffer);
    PARAM_UNUSED(size);
    return RwspError_Ok;
}
