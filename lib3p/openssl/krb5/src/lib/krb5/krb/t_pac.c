/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 * Copyright (c) 2006 Kungliga Tekniska Högskolan
 * (Royal Institute of Technology, Stockholm, Sweden).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "k5-int.h"

#define U(x) (uint8_t *)x

/*
 * This PAC and keys are copied (with permission) from Samba torture
 * regression test suite, they where created by Andrew Bartlet.
 */

static const unsigned char saved_pac[] = {
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xd8, 0x01, 0x00, 0x00,
    0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
    0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
    0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x08, 0x00, 0xcc, 0xcc, 0xcc, 0xcc,
    0xc8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x30, 0xdf, 0xa6, 0xcb,
    0x4f, 0x7d, 0xc5, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0xc0, 0x3c, 0x4e, 0x59, 0x62, 0x73, 0xc5, 0x01, 0xc0, 0x3c, 0x4e, 0x59,
    0x62, 0x73, 0xc5, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x16, 0x00, 0x16, 0x00,
    0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x14, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x02, 0x00, 0x65, 0x00, 0x00, 0x00,
    0xed, 0x03, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x02, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x16, 0x00, 0x20, 0x00, 0x02, 0x00, 0x16, 0x00, 0x18, 0x00,
    0x24, 0x00, 0x02, 0x00, 0x28, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x57, 0x00, 0x32, 0x00, 0x30, 0x00, 0x30, 0x00, 0x33, 0x00, 0x46, 0x00, 0x49, 0x00, 0x4e, 0x00,
    0x41, 0x00, 0x4c, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x57, 0x00, 0x32, 0x00,
    0x30, 0x00, 0x30, 0x00, 0x33, 0x00, 0x46, 0x00, 0x49, 0x00, 0x4e, 0x00, 0x41, 0x00, 0x4c, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x57, 0x00, 0x49, 0x00,
    0x4e, 0x00, 0x32, 0x00, 0x4b, 0x00, 0x33, 0x00, 0x54, 0x00, 0x48, 0x00, 0x49, 0x00, 0x4e, 0x00,
    0x4b, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
    0x15, 0x00, 0x00, 0x00, 0x11, 0x2f, 0xaf, 0xb5, 0x90, 0x04, 0x1b, 0xec, 0x50, 0x3b, 0xec, 0xdc,
    0x01, 0x00, 0x00, 0x00, 0x30, 0x00, 0x02, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x66, 0x28, 0xea, 0x37, 0x80, 0xc5, 0x01, 0x16, 0x00, 0x77, 0x00, 0x32, 0x00, 0x30, 0x00,
    0x30, 0x00, 0x33, 0x00, 0x66, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x24, 0x00,
    0x76, 0xff, 0xff, 0xff, 0x37, 0xd5, 0xb0, 0xf7, 0x24, 0xf0, 0xd6, 0xd4, 0xec, 0x09, 0x86, 0x5a,
    0xa0, 0xe8, 0xc3, 0xa9, 0x00, 0x00, 0x00, 0x00, 0x76, 0xff, 0xff, 0xff, 0xb4, 0xd8, 0xb8, 0xfe,
    0x83, 0xb3, 0x13, 0x3f, 0xfc, 0x5c, 0x41, 0xad, 0xe2, 0x64, 0x83, 0xe0, 0x00, 0x00, 0x00, 0x00
};

static unsigned int type_1_length = 472;

static const krb5_keyblock kdc_keyblock = {
    0, ENCTYPE_ARCFOUR_HMAC,
    16, U("\xB2\x86\x75\x71\x48\xAF\x7F\xD2\x52\xC5\x36\x03\xA1\x50\xB7\xE7")
};

static const krb5_keyblock member_keyblock = {
    0, ENCTYPE_ARCFOUR_HMAC,
    16, U("\xD2\x17\xFA\xEA\xE5\xE6\xB5\xF9\x5C\xCC\x94\x07\x7A\xB8\xA5\xFC")
};

static time_t authtime = 1120440609;
static const char *user = "w2003final$@WIN2K3.THINKER.LOCAL";

/* The S4U2Self PACs below were collected by debugging krb5-mit code on
 * Linux, talking with a Windows 2008 KDC server over the network. */

static const unsigned char s4u_pac_regular[] = {
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00,
    0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0a, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
    0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
    0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x48, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
    0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x10, 0x08, 0x00, 0xcc, 0xcc, 0xcc, 0xcc,
    0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0xc9, 0x36, 0xfd, 0x57,
    0x5b, 0x59, 0xd4, 0x01, 0xc9, 0x36, 0xfd, 0x57,
    0x5b, 0x59, 0xd4, 0x01, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0x0a, 0x00, 0x0a, 0x00,
    0x04, 0x00, 0x02, 0x00, 0x0a, 0x00, 0x0a, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x14, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x76, 0x04, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x02, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x00,
    0x20, 0x00, 0x02, 0x00, 0x08, 0x00, 0x0a, 0x00,
    0x24, 0x00, 0x02, 0x00, 0x28, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x01, 0x02, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x57, 0x00, 0x44, 0x00,
    0x43, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x41, 0x00, 0x43, 0x00, 0x4d, 0x00, 0x45, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x05, 0x15, 0x00, 0x00, 0x00,
    0x74, 0xa0, 0x8d, 0x00, 0x3f, 0xa5, 0xc2, 0xe9,
    0x60, 0x91, 0xe1, 0x22, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x89, 0xa1, 0x25, 0xd0, 0x59, 0xd4, 0x01,
    0x0a, 0x00, 0x77, 0x00, 0x32, 0x00, 0x6b, 0x00,
    0x38, 0x00, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x12, 0x00, 0x10, 0x00, 0x10, 0x00, 0x28, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x40, 0x00, 0x61, 0x00, 0x62, 0x00,
    0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x41, 0x00, 0x43, 0x00, 0x4d, 0x00, 0x45, 0x00,
    0x2e, 0x00, 0x43, 0x00, 0x4f, 0x00, 0x4d, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x88, 0x1d, 0x40, 0x84,
    0x7a, 0x01, 0x7c, 0x80, 0x74, 0xe3, 0x6a, 0x6b,
    0x76, 0xff, 0xff, 0xff, 0x1a, 0x1d, 0x97, 0xd2,
    0x39, 0xf4, 0xb8, 0xb2, 0x53, 0xae, 0x77, 0xdb,
    0x6c, 0x02, 0xd4, 0x3d, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char s4u_pac_enterprise[] = {
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00,
    0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0a, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
    0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
    0x18, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x50, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
    0x60, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x10, 0x08, 0x00, 0xcc, 0xcc, 0xcc, 0xcc,
    0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0xc9, 0x36, 0xfd, 0x57,
    0x5b, 0x59, 0xd4, 0x01, 0xc9, 0x36, 0xfd, 0x57,
    0x5b, 0x59, 0xd4, 0x01, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0x0a, 0x00, 0x0a, 0x00,
    0x04, 0x00, 0x02, 0x00, 0x0a, 0x00, 0x0a, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x14, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x76, 0x04, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x02, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x00,
    0x20, 0x00, 0x02, 0x00, 0x08, 0x00, 0x0a, 0x00,
    0x24, 0x00, 0x02, 0x00, 0x28, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x01, 0x02, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x57, 0x00, 0x44, 0x00,
    0x43, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x41, 0x00, 0x43, 0x00, 0x4d, 0x00, 0x45, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x05, 0x15, 0x00, 0x00, 0x00,
    0x74, 0xa0, 0x8d, 0x00, 0x3f, 0xa5, 0xc2, 0xe9,
    0x60, 0x91, 0xe1, 0x22, 0x00, 0x00, 0x00, 0x00,
    0x80, 0xe1, 0x9b, 0xe2, 0xe0, 0x59, 0xd4, 0x01,
    0x12, 0x00, 0x77, 0x00, 0x32, 0x00, 0x6b, 0x00,
    0x38, 0x00, 0x75, 0x00, 0x40, 0x00, 0x61, 0x00,
    0x62, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x12, 0x00, 0x10, 0x00, 0x10, 0x00, 0x28, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x40, 0x00, 0x61, 0x00, 0x62, 0x00,
    0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x41, 0x00, 0x43, 0x00, 0x4d, 0x00, 0x45, 0x00,
    0x2e, 0x00, 0x43, 0x00, 0x4f, 0x00, 0x4d, 0x00,
    0x10, 0x00, 0x00, 0x00, 0xfb, 0xe5, 0x03, 0x12,
    0x13, 0x00, 0x6c, 0x8e, 0x81, 0x97, 0x09, 0xea,
    0x76, 0xff, 0xff, 0xff, 0xba, 0xcd, 0x3a, 0xbc,
    0x67, 0x61, 0x16, 0x9f, 0xb8, 0x96, 0xbc, 0xe1,
    0xbe, 0x34, 0xe1, 0x77, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char s4u_pac_xrealm[] = {
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00,
    0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0a, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00,
    0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
    0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
    0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x10, 0x08, 0x00, 0xcc, 0xcc, 0xcc, 0xcc,
    0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0xc9, 0x36, 0xfd, 0x57,
    0x5b, 0x59, 0xd4, 0x01, 0xc9, 0x36, 0xfd, 0x57,
    0x5b, 0x59, 0xd4, 0x01, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0x0a, 0x00, 0x0a, 0x00,
    0x04, 0x00, 0x02, 0x00, 0x0a, 0x00, 0x0a, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x14, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x76, 0x04, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x02, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x00,
    0x20, 0x00, 0x02, 0x00, 0x08, 0x00, 0x0a, 0x00,
    0x24, 0x00, 0x02, 0x00, 0x28, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x01, 0x02, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x57, 0x00, 0x44, 0x00,
    0x43, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x41, 0x00, 0x43, 0x00, 0x4d, 0x00, 0x45, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x05, 0x15, 0x00, 0x00, 0x00,
    0x74, 0xa0, 0x8d, 0x00, 0x3f, 0xa5, 0xc2, 0xe9,
    0x60, 0x91, 0xe1, 0x22, 0x00, 0x00, 0x00, 0x00,
    0x80, 0xa8, 0x60, 0x1b, 0x2b, 0x5a, 0xd4, 0x01,
    0x1c, 0x00, 0x77, 0x00, 0x32, 0x00, 0x6b, 0x00,
    0x38, 0x00, 0x75, 0x00, 0x40, 0x00, 0x41, 0x00,
    0x43, 0x00, 0x4d, 0x00, 0x45, 0x00, 0x2e, 0x00,
    0x43, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x00, 0x00,
    0x12, 0x00, 0x10, 0x00, 0x10, 0x00, 0x28, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x40, 0x00, 0x61, 0x00, 0x62, 0x00,
    0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x41, 0x00, 0x43, 0x00, 0x4d, 0x00, 0x45, 0x00,
    0x2e, 0x00, 0x43, 0x00, 0x4f, 0x00, 0x4d, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x11, 0x27, 0x3a, 0xa5,
    0x41, 0x84, 0x87, 0xdf, 0xc6, 0xd7, 0x29, 0x26,
    0x76, 0xff, 0xff, 0xff, 0xba, 0x7c, 0x7a, 0x84,
    0xd2, 0x2b, 0x9c, 0x58, 0xed, 0x2f, 0xdf, 0x23,
    0x09, 0x15, 0x05, 0x6b, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char s4u_pac_ent_xrealm[] = {
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00,
    0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0a, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00,
    0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
    0x28, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x60, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
    0x70, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x10, 0x08, 0x00, 0xcc, 0xcc, 0xcc, 0xcc,
    0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0xc9, 0x36, 0xfd, 0x57,
    0x5b, 0x59, 0xd4, 0x01, 0xc9, 0x36, 0xfd, 0x57,
    0x5b, 0x59, 0xd4, 0x01, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x7f, 0x0a, 0x00, 0x0a, 0x00,
    0x04, 0x00, 0x02, 0x00, 0x0a, 0x00, 0x0a, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x14, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x76, 0x04, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x02, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x00,
    0x20, 0x00, 0x02, 0x00, 0x08, 0x00, 0x0a, 0x00,
    0x24, 0x00, 0x02, 0x00, 0x28, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x01, 0x02, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x57, 0x00, 0x44, 0x00,
    0x43, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x41, 0x00, 0x43, 0x00, 0x4d, 0x00, 0x45, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x05, 0x15, 0x00, 0x00, 0x00,
    0x74, 0xa0, 0x8d, 0x00, 0x3f, 0xa5, 0xc2, 0xe9,
    0x60, 0x91, 0xe1, 0x22, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x87, 0x39, 0x5b, 0x4f, 0x5a, 0xd4, 0x01,
    0x24, 0x00, 0x77, 0x00, 0x32, 0x00, 0x6b, 0x00,
    0x38, 0x00, 0x75, 0x00, 0x40, 0x00, 0x61, 0x00,
    0x62, 0x00, 0x63, 0x00, 0x40, 0x00, 0x41, 0x00,
    0x43, 0x00, 0x4d, 0x00, 0x45, 0x00, 0x2e, 0x00,
    0x43, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x00, 0x00,
    0x12, 0x00, 0x10, 0x00, 0x10, 0x00, 0x28, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x32, 0x00, 0x6b, 0x00, 0x38, 0x00,
    0x75, 0x00, 0x40, 0x00, 0x61, 0x00, 0x62, 0x00,
    0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x41, 0x00, 0x43, 0x00, 0x4d, 0x00, 0x45, 0x00,
    0x2e, 0x00, 0x43, 0x00, 0x4f, 0x00, 0x4d, 0x00,
    0x10, 0x00, 0x00, 0x00, 0xa3, 0x5d, 0xc5, 0xfe,
    0x80, 0x6b, 0x62, 0x0c, 0xb1, 0x2f, 0x43, 0xa2,
    0x76, 0xff, 0xff, 0xff, 0x95, 0x40, 0x76, 0xe4,
    0x0a, 0x0a, 0xb9, 0xe7, 0x93, 0x0f, 0x05, 0xf8,
    0x8a, 0x81, 0x9c, 0x9c, 0x00, 0x00, 0x00, 0x00
};

static const char *s4u_principal = "w2k8u@ACME.COM";
static const char *s4u_enterprise = "w2k8u@abc@ACME.COM";

static const krb5_keyblock s4u_srv_key = {
    0, ENCTYPE_AES256_CTS_HMAC_SHA1_96,
    32, U("\x14\xDF\xB5\xB2\xCD\xB4\x2C\x88\x94\xDA\x2F\xA8\x82\xE9\x72\x9F"
          "\x4A\x4D\xC7\x4B\xA0\x2A\x24\x2C\xC6\xA8\xD7\x10\x79\xB9\xAD\x9A")
};

static const krb5_keyblock s4u_tgt_srv_key = {
    0, ENCTYPE_AES256_CTS_HMAC_SHA1_96,
    32, U("\x42\x0C\x39\xC5\x1A\x17\x54\x04\x45\x1F\x95\x6B\x8C\x58\xE0\xF4"
          "\x1B\xCA\x66\x9A\x64\x47\x95\xCA\x6E\x3A\xD5\x5A\x3B\x91\x8C\x9F")
};

static size_t s4u_logon_info_buffer_len = 416;

struct pac_and_info {
    time_t authtime;
    krb5_boolean is_enterprise;
    krb5_boolean is_xrealm;
    const unsigned char *data;
    size_t length;
};

static const struct pac_and_info s4u_pacs[] = {
    { 1538430362, 0, 0, s4u_pac_regular, sizeof(s4u_pac_regular) },
    { 1538437551, 1, 0, s4u_pac_enterprise, sizeof(s4u_pac_enterprise) },
    { 1538469429, 0, 1, s4u_pac_xrealm, sizeof(s4u_pac_xrealm) },
    { 1538484998, 1, 1, s4u_pac_ent_xrealm, sizeof(s4u_pac_ent_xrealm) },
    { 0, 0, 0, NULL, 0 }
};

#if !defined(__cplusplus) && (__GNUC__ > 2)
static void err(krb5_context ctx, krb5_error_code code, const char *fmt, ...)
    __attribute__((__format__(__printf__, 3, 0)));
#endif

static void
err(krb5_context ctx, krb5_error_code code, const char *fmt, ...)
{
    va_list ap;
    char *msg;
    const char *errmsg = NULL;

    va_start(ap, fmt);
    if (vasprintf(&msg, fmt, ap) < 0)
        exit(1);
    va_end(ap);
    if (ctx && code)
        errmsg = krb5_get_error_message(ctx, code);
    if (errmsg)
        fprintf(stderr, "t_pac: %s: %s\n", msg, errmsg);
    else
        fprintf(stderr, "t_pac: %s\n", msg);
    exit(1);
}

static void
check_pac(krb5_context context, int index, const unsigned char *pdata,
          size_t plen, time_t auth_time, krb5_principal p,
          size_t type_one_buffer_length, krb5_boolean with_realm,
          const krb5_keyblock *server_key, const krb5_keyblock *kdc_key)
{
    krb5_error_code ret;
    const krb5_keyblock *kdc_sign_key;
    krb5_data data;
    krb5_pac pac;

    /* If we don't have the KDC key (S4U cases), just use another key as we'd
     * skip the KDC signature when verifying. */
    kdc_sign_key = (kdc_key == NULL) ? &kdc_keyblock : kdc_key;

    ret = krb5_pac_parse(context, pdata, plen, &pac);
    if (ret)
        err(context, ret, "[pac: %d] krb5_pac_parse", index);

    ret = krb5_pac_verify_ext(context, pac, auth_time, p, server_key, kdc_key,
                              with_realm);
    if (ret)
        err(context, ret, "[pac: %d] krb5_pac_verify_ext", index);

    ret = krb5_pac_sign_ext(context, pac, auth_time, p, server_key,
                            kdc_sign_key, with_realm, &data);
    if (ret)
        err(context, ret, "[pac: %d] krb5_pac_sign_ext", index);

    krb5_pac_free(context, pac);

    ret = krb5_pac_parse(context, data.data, data.length, &pac);
    krb5_free_data_contents(context, &data);
    if (ret)
        err(context, ret, "[pac: %d] krb5_pac_parse 2", index);

    ret = krb5_pac_verify_ext(context, pac, auth_time, p, server_key, kdc_key,
                              with_realm);
    if (ret)
        err(context, ret, "[pac: %d] krb5_pac_verify_ext 2", index);

    /* make a copy and try to reproduce it */
    {
        uint32_t *list;
        size_t len, i;
        krb5_pac pac2;

        ret = krb5_pac_init(context, &pac2);
        if (ret)
            err(context, ret, "[pac: %d] krb5_pac_init", index);

        /* our two user buffer plus the three "system" buffers */
        ret = krb5_pac_get_types(context, pac, &len, &list);
        if (ret)
            err(context, ret, "[pac: %d] krb5_pac_get_types", index);

        for (i = 0; i < len; i++) {
            /* skip server_cksum, privsvr_cksum, and logon_name */
            if (list[i] == 6 || list[i] == 7 || list[i] == 10)
                continue;

            ret = krb5_pac_get_buffer(context, pac, list[i], &data);
            if (ret)
                err(context, ret, "[pac: %d] krb5_pac_get_buffer", index);

            if (list[i] == 1) {
                if (type_one_buffer_length != data.length) {
                    err(context, 0, "[pac: %d] type 1 have wrong length: %lu",
                        index, (unsigned long)data.length);
                }
            } else if (list[i] != 12) {
                err(context, 0, "[pac: %d] unknown type %lu",
                    index, (unsigned long)list[i]);
            }

            ret = krb5_pac_add_buffer(context, pac2, list[i], &data);
            if (ret)
                err(context, ret, "[pac: %d] krb5_pac_add_buffer", index);
            krb5_free_data_contents(context, &data);
        }
        free(list);

        ret = krb5_pac_sign_ext(context, pac2, auth_time, p, server_key,
                                kdc_sign_key, with_realm, &data);
        if (ret)
            err(context, ret, "[pac: %d] krb5_pac_sign_ext 4", index);

        krb5_pac_free(context, pac2);

        ret = krb5_pac_parse(context, data.data, data.length, &pac2);
        if (ret)
            err(context, ret, "[pac: %d] krb5_pac_parse 4", index);

        ret = krb5_pac_verify_ext(context, pac2, auth_time, p, server_key,
                                  kdc_key, with_realm);
        if (ret)
            err(context, ret, "[pac: %d] krb5_pac_verify_ext 4", index);

        krb5_free_data_contents(context, &data);

        krb5_pac_free(context, pac2);
    }

    krb5_pac_free(context, pac);
}

int
main(int argc, char **argv)
{
    krb5_error_code ret;
    krb5_context context;
    krb5_pac pac;
    krb5_data data;
    krb5_principal p;

    ret = krb5_init_context(&context);
    if (ret)
        err(NULL, 0, "krb5_init_contex");

    ret = krb5_set_default_realm(context, "WIN2K3.THINKER.LOCAL");
    if (ret)
        err(context, ret, "krb5_set_default_realm");

    ret = krb5_parse_name(context, user, &p);
    if (ret)
        err(context, ret, "krb5_parse_name");

    /* Check a pre-saved PAC. */
    check_pac(context, -1, saved_pac, sizeof(saved_pac), authtime, p,
              type_1_length, 0, &member_keyblock, &kdc_keyblock);

    /* Check S4U2Self PACs. */
    {
        krb5_principal sp;
        krb5_principal sep;
        const struct pac_and_info *pi;

        ret = krb5_parse_name(context, s4u_principal, &sp);
        if (ret)
            err(context, ret, "krb5_parse_name");

        ret = krb5_parse_name_flags(context, s4u_enterprise,
                                    KRB5_PRINCIPAL_PARSE_ENTERPRISE, &sep);
        if (ret)
            err(context, ret, "krb5_parse_name_flags");

        for (pi = s4u_pacs; pi->data != NULL; pi++) {
            check_pac(context, pi - s4u_pacs, pi->data, pi->length,
                      pi->authtime, pi->is_enterprise ? sep : sp,
                      s4u_logon_info_buffer_len, pi->is_xrealm,
                      pi->is_xrealm ? &s4u_tgt_srv_key : &s4u_srv_key, NULL);
        }

        krb5_free_principal(context, sp);
        krb5_free_principal(context, sep);
    }

    /*
     * Test empty free
     */

    ret = krb5_pac_init(context, &pac);
    if (ret)
        err(context, ret, "krb5_pac_init");
    krb5_pac_free(context, pac);

    /*
     * Test add remove buffer
     */

    ret = krb5_pac_init(context, &pac);
    if (ret)
        err(context, ret, "krb5_pac_init");

    {
        const krb5_data cdata = { 0, 2, "\x00\x01" } ;

        ret = krb5_pac_add_buffer(context, pac, 1, &cdata);
        if (ret)
            err(context, ret, "krb5_pac_add_buffer");
    }
    {
        ret = krb5_pac_get_buffer(context, pac, 1, &data);
        if (ret)
            err(context, ret, "krb5_pac_get_buffer");
        if (data.length != 2 || memcmp(data.data, "\x00\x01", 2) != 0)
            err(context, 0, "krb5_pac_get_buffer data not the same");
        krb5_free_data_contents(context, &data);
    }

    {
        const krb5_data cdata = { 0, 2, "\x02\x00" } ;

        ret = krb5_pac_add_buffer(context, pac, 2, &cdata);
        if (ret)
            err(context, ret, "krb5_pac_add_buffer");
    }
    {
        ret = krb5_pac_get_buffer(context, pac, 1, &data);
        if (ret)
            err(context, ret, "krb5_pac_get_buffer");
        if (data.length != 2 || memcmp(data.data, "\x00\x01", 2) != 0)
            err(context, 0, "krb5_pac_get_buffer data not the same");
        krb5_free_data_contents(context, &data);
        /* */
        ret = krb5_pac_get_buffer(context, pac, 2, &data);
        if (ret)
            err(context, ret, "krb5_pac_get_buffer");
        if (data.length != 2 || memcmp(data.data, "\x02\x00", 2) != 0)
            err(context, 0, "krb5_pac_get_buffer data not the same");
        krb5_free_data_contents(context, &data);
    }

    ret = krb5_pac_sign(context, pac, authtime, p,
                        &member_keyblock, &kdc_keyblock, &data);
    if (ret)
        err(context, ret, "krb5_pac_sign");

    krb5_pac_free(context, pac);

    ret = krb5_pac_parse(context, data.data, data.length, &pac);
    krb5_free_data_contents(context, &data);
    if (ret)
        err(context, ret, "krb5_pac_parse 3");

    ret = krb5_pac_verify(context, pac, authtime, p,
                          &member_keyblock, &kdc_keyblock);
    if (ret)
        err(context, ret, "krb5_pac_verify 3");

    {
        uint32_t *list;
        size_t len;

        /* our two user buffer plus the three "system" buffers */
        ret = krb5_pac_get_types(context, pac, &len, &list);
        if (ret)
            err(context, ret, "krb5_pac_get_types");
        if (len != 5)
            err(context, 0, "list wrong length");
        free(list);
    }

    {
        krb5_principal ep;

        ret = krb5_parse_name_flags(context, user,
                                    KRB5_PRINCIPAL_PARSE_ENTERPRISE, &ep);
        if (ret)
            err(context, ret, "krb5_parse_name_flags");

        /* Try to verify as enterprise. */
        ret = krb5_pac_verify(context, pac, authtime, ep, &member_keyblock,
                              &kdc_keyblock);
        if (!ret)
            err(context, ret, "krb5_pac_verify should have failed");

        ret = krb5_pac_sign(context, pac, authtime, ep, &member_keyblock,
                            &kdc_keyblock, &data);
        if (!ret)
            err(context, ret, "krb5_pac_sign should have failed");

        /* Try to verify with realm. */
        ret = krb5_pac_verify_ext(context, pac, authtime, p, &member_keyblock,
                                  &kdc_keyblock, TRUE);
        if (!ret)
            err(context, ret, "krb5_pac_verify_ext with realm should fail");

        /* Currently we can't re-sign the PAC with realm (although that could
         * be useful), only sign a new one. */
        ret = krb5_pac_sign_ext(context, pac, authtime, p, &member_keyblock,
                                &kdc_keyblock, TRUE, &data);
        if (!ret)
            err(context, ret, "krb5_pac_sign_ext with realm should fail");

        krb5_pac_free(context, pac);

        /* Test enterprise. */
        ret = krb5_pac_init(context, &pac);
        if (ret)
            err(context, ret, "krb5_pac_init");

        ret = krb5_pac_sign(context, pac, authtime, ep, &member_keyblock,
                            &kdc_keyblock, &data);
        if (ret)
            err(context, ret, "krb5_pac_sign enterprise failed");

        krb5_pac_free(context, pac);

        ret = krb5_pac_parse(context, data.data, data.length, &pac);
        krb5_free_data_contents(context, &data);
        if (ret)
            err(context, ret, "krb5_pac_parse failed");

        ret = krb5_pac_verify(context, pac, authtime, ep, &member_keyblock,
                              &kdc_keyblock);
        if (ret)
            err(context, ret, "krb5_pac_verify enterprise failed");

        ret = krb5_pac_verify(context, pac, authtime, p, &member_keyblock,
                              &kdc_keyblock);
        if (!ret)
            err(context, ret, "krb5_pac_verify should have failed");

        krb5_pac_free(context, pac);

        /* Test with realm. */
        ret = krb5_pac_init(context, &pac);
        if (ret)
            err(context, ret, "krb5_pac_init");

        ret = krb5_pac_sign_ext(context, pac, authtime, p, &member_keyblock,
                                &kdc_keyblock, TRUE, &data);
        if (ret)
            err(context, ret, "krb5_pac_sign_ext with realm failed");

        krb5_pac_free(context, pac);

        ret = krb5_pac_parse(context, data.data, data.length, &pac);
        krb5_free_data_contents(context, &data);
        if (ret)
            err(context, ret, "krb5_pac_parse failed");

        ret = krb5_pac_verify_ext(context, pac, authtime, p, &member_keyblock,
                                  &kdc_keyblock, TRUE);
        if (ret)
            err(context, ret, "krb5_pac_verify_ext with realm failed");

        ret = krb5_pac_verify(context, pac, authtime, p, &member_keyblock,
                              &kdc_keyblock);
        if (!ret)
            err(context, ret, "krb5_pac_verify should have failed");

        krb5_pac_free(context, pac);

        /* Test enterprise with realm. */
        ret = krb5_pac_init(context, &pac);
        if (ret)
            err(context, ret, "krb5_pac_init");

        ret = krb5_pac_sign_ext(context, pac, authtime, ep, &member_keyblock,
                                &kdc_keyblock, TRUE, &data);
        if (ret)
            err(context, ret, "krb5_pac_sign_ext ent with realm failed");

        krb5_pac_free(context, pac);

        ret = krb5_pac_parse(context, data.data, data.length, &pac);
        krb5_free_data_contents(context, &data);
        if (ret)
            err(context, ret, "krb5_pac_parse failed");

        ret = krb5_pac_verify_ext(context, pac, authtime, ep, &member_keyblock,
                                  &kdc_keyblock, TRUE);
        if (ret)
            err(context, ret, "krb5_pac_verify_ext ent with realm failed");

        ret = krb5_pac_verify(context, pac, authtime, p, &member_keyblock,
                              &kdc_keyblock);
        if (!ret)
            err(context, ret, "krb5_pac_verify should have failed");

        ret = krb5_pac_verify(context, pac, authtime, ep, &member_keyblock,
                              &kdc_keyblock);
        if (!ret)
            err(context, ret, "krb5_pac_verify should have failed");

        ret = krb5_pac_verify_ext(context, pac, authtime, p, &member_keyblock,
                                  &kdc_keyblock, TRUE);
        if (!ret)
            err(context, ret, "krb5_pac_verify_ext should have failed");

        krb5_free_principal(context, ep);
    }

    krb5_pac_free(context, pac);

    krb5_free_principal(context, p);
    krb5_free_context(context);

    return 0;
}
