/** @file noja_webserver_mem.h
 * Simple memory managment module header file
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
#ifndef NOJA_WEBSERVER_MEM_H
#define NOJA_WEBSERVER_MEM_H

#include "noja_webserver_cmn.h"

#if defined(NWS_MEMORY_ALLOC_DBG)
#define NWSALLOC(x, y) nwsAlloc((x), (y), __FUNCTION__, __LINE__)
#define NWSFREE(x, y, z) nwsFree((x), (void*)(y), (z), __FUNCTION__, __LINE__)
#else
#define NWSALLOC(x, y) nwsAlloc((x), (y))
#define NWSFREE(x, y, z) nwsFree((x), (void*)(y), (z))
#endif

/** @ingroup group_internal
  * @brief allocate heap memory
  *
  * This function allocate a specified heap size
  *
  * @param[in] ctx Webserver context that requested the allocated memory
  * @param[in] size Size of meomory to allocate
  * @return Pointer to the allocated memory, NULL if failed
  */
extern void* nwsAlloc(NwsContextInternal *ctx, unsigned int size
#if defined(NWS_MEMORY_ALLOC_DBG)
    , const char *function, unsigned int line
#endif
  );

/** @ingroup group_internal
  * @brief free allocated heap memory
  *
  * This function free an allocated memory
  *
  * @param[in] ctx Webserver context that requested the allocated memory
  * @param[in] buf pointer to the memory to free
  * @param[in] size Size of meomory to free
  */
extern void nwsFree(NwsContextInternal *ctx, void* buf, unsigned int size
#if defined(NWS_MEMORY_ALLOC_DBG)
    , const char *function, unsigned int line
#endif
  );

#endif
