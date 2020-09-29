/** @file noja_webserver_mem.c
 * Simple memory managment module implementation
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

#include "noja_webserver_mem.h"


/** @ingroup group_internal
  * @brief allocate heap memory
  *
  * This function allocate a specified heap size
  *
  * @param[in] ctx Webserver context that requested the allocated memory
  * @param[in] size Size of meomory to allocate
  * @return Pointer to the allocated memory, NULL if failed
  */
void* nwsAlloc(NwsContextInternal *ctx, unsigned int size
#if defined(NWS_MEMORY_ALLOC_DBG)
    , const char *function, unsigned int line
#endif
    )
{
    void *buf = NULL;
    buf = malloc(size);
    if(buf == NULL) {
        SYSERR(-ERR_WEBSERVER, "Unable to malloc %d bytes", size);
        return buf;
    }

    if(ctx) {
        ctx->memAllocCnt += 1;
        ctx->memAllocSize += size;
        if(ctx->memAllocSize > ctx->memHighSize) {
            ctx->memHighSize = ctx->memAllocSize;
        }
#if defined(NWS_MEMORY_ALLOC_DBG)
        fprintf(stderr, "Alloc[%s.%d] Address=%08X, Size=%d, Count=%d, Total Size: %d\n", function, line,
            (unsigned int)buf, size, ctx->memAllocCnt, ctx->memAllocSize);
#endif
    }

    return buf;
}

/** @ingroup group_internal
  * @brief free allocated heap memory
  *
  * This function free an allocated memory
  *
  * @param[in] ctx Webserver context that requested the allocated memory
  * @param[in] buf pointer to the memory to free
  * @param[in] size Size of meomory to free
  */
void nwsFree(NwsContextInternal *ctx, void* buf, unsigned int size
#if defined(NWS_MEMORY_ALLOC_DBG)
    , const char *function, unsigned int line
#endif
    )
{
    if(buf == NULL) {
        return;
    }
    if(ctx) {
        ctx->memAllocCnt -= 1;
        ctx->memAllocSize -= size;
#if defined(NWS_MEMORY_ALLOC_DBG)
        fprintf(stderr, "Free[%s.%d] Address=%08X, Size=%d, Count=%d, Total Size: %d\n", function, line,
            (unsigned int)buf, size, ctx->memAllocCnt, ctx->memAllocSize);
#endif
    }
    free(buf);
}