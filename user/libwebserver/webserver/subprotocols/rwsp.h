/** @file rwsp.h
 * Protocol header file, subprotocol needs this header to access the noja webserver
 * protocol
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
#ifndef _RWSP_H
#define _RWSP_H

#include "rwsp_type.h"

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
RwspErrors rwspReg(const char* name, RwspRcvdCb rcvdCb, RwspEncCb encCb, RwspDecCb decCb);
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
RwspErrors rwspSnd(RwspCtx *context, RwspData data, RwspDataSize size);
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
RwspErrors rwspBcast(RwspData data, RwspDataSize size);
#endif