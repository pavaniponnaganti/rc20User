/* ccapi/lib/win/ccs_reply_proc.c */
/*
 * Copyright 2008 Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 * require a specific license from the United States Government.
 * It is the responsibility of any person or organization contemplating
 * export to obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  Furthermore if you modify this software you must label
 * your software as modified software and not distribute it in such a
 * fashion that it might be confused with the original M.I.T. software.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "cci_debugging.h"
#include "ccs_reply.h"          /* generated by MIDL compiler */
#include "ccutils.h"
#include "tls.h"
#include "win-utils.h"


void ccs_rpc_request_reply(
    const long  rpcmsg,         /* Message type */
    const char  tspHandle[],    /* Client's tspdata* */
    const char* uuid,           /* uuid for making thread-specific event name */
    const long  srvStartTime,   /* Server Start Time */
    const long  cbIn,           /* Length of buffer */
    const char* chIn,           /* Data buffer */
    long*       ret_status ) {  /* Return code */

    HANDLE          hEvent  = openThreadEvent(uuid, REPLY_SUFFIX);
    struct tspdata* tsp;
    k5_ipc_stream    stream;
    long            status  = 0;

    memcpy(&tsp, tspHandle, sizeof(tsp));
    if (!status) {
        status = krb5int_ipc_stream_new (&stream);  /* Create a stream for the request data */
        }

    if (!status) {                          /* Put the data into the stream */
        status = krb5int_ipc_stream_write (stream, chIn, cbIn);
        }

    if (!status) {                          /* Put the data into the stream */
        tspdata_setStream(tsp, stream);
        }

    SetEvent(hEvent);
    CloseHandle(hEvent);
    *ret_status  = status;
    }

void ccs_rpc_connect_reply(
    const long  rpcmsg,         /* Message type */
    const char  tspHandle[],    /* Client's tspdata* */
    const char* uuid,           /* uuid for making thread-specific event name */
    const long  srvStartTime,   /* Server Start Time */
    long*       status ) {      /* Return code */

    HANDLE  hEvent  = openThreadEvent(uuid, REPLY_SUFFIX);
    DWORD*  p       = (DWORD*)(tspHandle);

    SetEvent(hEvent);
    CloseHandle(hEvent);
    }

void ccapi_listen(
    RPC_ASYNC_STATE*    rpcState,
    handle_t            hBinding,
    const long          rpcmsg,         /* Message type */
    long*               status ) {      /* Return code */

    cci_debug_printf("%s %s!", __FUNCTION__, rpcState->UserInfo);
    *status = 0;
    }
