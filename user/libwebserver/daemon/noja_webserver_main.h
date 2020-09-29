/** @file noja_webserver_main.h
 * Webserver daemon header file
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
#ifndef _NOJA_WEBSERVER_MAIN_H
#define _NOJA_WEBSERVER_MAIN_H

#if defined(QT_CORE_LIB)
    #if defined(SYS_INFO)
        #undef SYS_INFO
    #endif
    #if defined(SYSERR)
        #undef SYSERR
    #endif
    #define SYS_INFO 0
    #define SYSERR(x, ...) { fprintf(stdout, __VA_ARGS__); fflush(stdout); }
#endif

#endif
