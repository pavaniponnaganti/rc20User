/* Copyright (c) 2015, Google Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pkcs8.h>
#include <openssl/x509.h>

#include "../internal.h"


/* kDER is a PKCS#8 encrypted private key. It was generated with:
 *
 * openssl genrsa 512 > test.key
 * openssl pkcs8 -topk8 -in test.key -out test.key.encrypted -v2 des3 -outform der
 * hexdump -Cv test.key.encrypted
 *
 * The password is "testing".
 */
static const uint8_t kDER[] = {
  0x30, 0x82, 0x01, 0x9e, 0x30, 0x40, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x05,
  0x0d, 0x30, 0x33, 0x30, 0x1b, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x05, 0x0c,
  0x30, 0x0e, 0x04, 0x08, 0x06, 0xa5, 0x4b, 0x0c, 0x0c, 0x50, 0x8c, 0x19, 0x02, 0x02, 0x08, 0x00,
  0x30, 0x14, 0x06, 0x08, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x03, 0x07, 0x04, 0x08, 0x3a, 0xd0,
  0x70, 0x4b, 0x26, 0x50, 0x13, 0x7b, 0x04, 0x82, 0x01, 0x58, 0xa6, 0xee, 0x02, 0xf2, 0xf2, 0x7c,
  0x19, 0x91, 0xe3, 0xce, 0x32, 0x85, 0xc5, 0x01, 0xd9, 0xe3, 0x5e, 0x14, 0xb6, 0xb8, 0x78, 0xad,
  0xda, 0x01, 0xec, 0x9e, 0x42, 0xe8, 0xbf, 0x0b, 0x46, 0x03, 0xbc, 0x92, 0x6f, 0xe4, 0x0f, 0x0f,
  0x48, 0x30, 0x10, 0x10, 0x9b, 0xfb, 0x4b, 0xb9, 0x45, 0xf8, 0xcf, 0xab, 0xa1, 0x18, 0xdd, 0x19,
  0xa4, 0xa4, 0xe1, 0xf0, 0xa1, 0x8d, 0xc2, 0x23, 0xe7, 0x0d, 0x7a, 0x64, 0x21, 0x6b, 0xfa, 0x48,
  0xb9, 0x41, 0xc1, 0x0c, 0x4b, 0xce, 0x6f, 0x1a, 0x91, 0x9b, 0x9f, 0xdd, 0xcf, 0xa9, 0x8d, 0x33,
  0x2c, 0x45, 0x81, 0x5c, 0x5e, 0x67, 0xc6, 0x68, 0x43, 0x62, 0xff, 0x5e, 0x9b, 0x1a, 0x15, 0x3a,
  0x9d, 0x71, 0x3f, 0xbe, 0x32, 0x2f, 0xe5, 0x90, 0x65, 0x65, 0x9c, 0x22, 0xf6, 0x29, 0x2e, 0xcf,
  0x26, 0x16, 0x7b, 0x66, 0x48, 0x55, 0xad, 0x9a, 0x8d, 0x89, 0xf4, 0x48, 0x4f, 0x1f, 0x9d, 0xb8,
  0xfa, 0xe1, 0xf1, 0x3b, 0x39, 0x5c, 0x72, 0xc6, 0xb8, 0x3e, 0x98, 0xe8, 0x77, 0xe8, 0xb6, 0x71,
  0x84, 0xa8, 0x6e, 0xca, 0xaf, 0x62, 0x96, 0x49, 0x8a, 0x21, 0x6f, 0x9e, 0x78, 0x07, 0x97, 0x38,
  0x40, 0x66, 0x42, 0x5a, 0x1b, 0xe0, 0x9b, 0xe9, 0x91, 0x82, 0xe4, 0xea, 0x8f, 0x2a, 0xb2, 0x80,
  0xce, 0xe8, 0x57, 0xd3, 0xac, 0x11, 0x9d, 0xb2, 0x39, 0x0f, 0xe1, 0xce, 0x18, 0x96, 0x38, 0xa1,
  0x19, 0x80, 0x88, 0x81, 0x3d, 0xda, 0xaa, 0x8e, 0x15, 0x27, 0x19, 0x73, 0x0c, 0xf3, 0xaf, 0x45,
  0xe9, 0x1b, 0xad, 0x6c, 0x3d, 0xbf, 0x95, 0xf7, 0xa0, 0x87, 0x0e, 0xde, 0xf1, 0xd8, 0xee, 0xaa,
  0x92, 0x76, 0x8d, 0x32, 0x45, 0xa1, 0xe7, 0xf5, 0x05, 0xd6, 0x2c, 0x67, 0x63, 0x10, 0xfa, 0xde,
  0x80, 0xc7, 0x5b, 0x96, 0x0f, 0x24, 0x50, 0x78, 0x30, 0xe5, 0x89, 0xf3, 0x73, 0xfa, 0x40, 0x11,
  0xd5, 0x26, 0xb8, 0x36, 0x96, 0x98, 0xe6, 0xbd, 0x73, 0x62, 0x56, 0xb9, 0xea, 0x28, 0x16, 0x93,
  0x5b, 0x33, 0xae, 0x83, 0xf9, 0x1f, 0xee, 0xef, 0xc8, 0xbf, 0xc7, 0xb1, 0x47, 0x43, 0xa1, 0xc6,
  0x1a, 0x64, 0x47, 0x02, 0x40, 0x3e, 0xbc, 0x0f, 0x80, 0x71, 0x5c, 0x44, 0x60, 0xbc, 0x78, 0x2e,
  0xd2, 0x77, 0xf8, 0x6e, 0x12, 0x51, 0x89, 0xdb, 0x90, 0x64, 0xcd, 0x76, 0x10, 0x29, 0x73, 0xc2,
  0x2f, 0x94, 0x7b, 0x98, 0xcd, 0xbb, 0x61, 0x16, 0x1d, 0x52, 0x11, 0x73, 0x48, 0xe6, 0x39, 0xfc,
  0xd6, 0x2d,
};

/* kNullPassword is a PKCS#8 encrypted private key using the null password. */
static const uint8_t kNullPassword[] = {
    0x30, 0x81, 0xb0, 0x30, 0x1b, 0x06, 0x0a, 0x2a, 0x86, 0x48, 0x86, 0xf7,
    0x0d, 0x01, 0x0c, 0x01, 0x03, 0x30, 0x0d, 0x04, 0x08, 0xb2, 0xfe, 0x68,
    0xc2, 0xea, 0x0f, 0x10, 0x9c, 0x02, 0x01, 0x01, 0x04, 0x81, 0x90, 0xe2,
    0xf6, 0x1c, 0xca, 0xad, 0x64, 0x30, 0xbf, 0x88, 0x04, 0x35, 0xe5, 0x0f,
    0x11, 0x49, 0x06, 0x01, 0x14, 0x33, 0x80, 0xa2, 0x78, 0x44, 0x5b, 0xaa,
    0x0d, 0xd7, 0x00, 0x36, 0x9d, 0x91, 0x97, 0x37, 0x20, 0x7b, 0x27, 0xc1,
    0xa0, 0xa2, 0x73, 0x06, 0x15, 0xdf, 0xc8, 0x13, 0x9b, 0xc9, 0x8c, 0x9c,
    0xce, 0x00, 0xd0, 0xc8, 0x42, 0xc1, 0xda, 0x2b, 0x07, 0x2b, 0x12, 0xa3,
    0xce, 0x10, 0x39, 0x7a, 0xf1, 0x55, 0x69, 0x8d, 0xa5, 0xc4, 0x2a, 0x00,
    0x0d, 0x94, 0xc6, 0xde, 0x6a, 0x3d, 0xb7, 0xe5, 0x6d, 0x59, 0x3e, 0x09,
    0xb5, 0xe3, 0x3e, 0xfc, 0x50, 0x56, 0xe9, 0x50, 0x42, 0x7c, 0xe7, 0xf0,
    0x19, 0xbd, 0x31, 0xa7, 0x85, 0x47, 0xb3, 0xe9, 0xb3, 0x50, 0x3c, 0xc9,
    0x32, 0x37, 0x1a, 0x93, 0x78, 0x48, 0x78, 0x82, 0xde, 0xad, 0x5c, 0xf2,
    0xcf, 0xf2, 0xbb, 0x2c, 0x44, 0x05, 0x7f, 0x4a, 0xf9, 0xb1, 0x2b, 0xdd,
    0x49, 0xf6, 0x7e, 0xd0, 0x42, 0xaa, 0x14, 0x3c, 0x24, 0x77, 0xb4,
};

/* kNullPasswordNSS is a PKCS#8 encrypted private key using the null password
 * and generated by NSS. */
static const uint8_t kNullPasswordNSS[] = {
    0x30, 0x81, 0xb8, 0x30, 0x23, 0x06, 0x0a, 0x2a, 0x86, 0x48, 0x86, 0xf7,
    0x0d, 0x01, 0x0c, 0x01, 0x03, 0x30, 0x15, 0x04, 0x10, 0x3f, 0xac, 0xe9,
    0x38, 0xdb, 0x40, 0x6b, 0x26, 0x89, 0x09, 0x73, 0x18, 0x8d, 0x7f, 0x1c,
    0x82, 0x02, 0x01, 0x01, 0x04, 0x81, 0x90, 0x5e, 0x5e, 0x11, 0xef, 0xbb,
    0x7c, 0x4d, 0xec, 0xc0, 0xdc, 0xc7, 0x23, 0xd2, 0xc4, 0x77, 0xbc, 0xf4,
    0x5d, 0x59, 0x4c, 0x07, 0xc2, 0x8a, 0x26, 0xfa, 0x25, 0x1c, 0xaa, 0x42,
    0xed, 0xd0, 0xed, 0xbb, 0x5c, 0xe9, 0x13, 0x07, 0xaa, 0xdd, 0x52, 0x3c,
    0x65, 0x25, 0xbf, 0x94, 0x02, 0xaf, 0xd6, 0x97, 0xe9, 0x33, 0x00, 0x76,
    0x64, 0x4a, 0x73, 0xab, 0xfb, 0x99, 0x6e, 0x83, 0x12, 0x05, 0x86, 0x72,
    0x6c, 0xd5, 0xa4, 0xcf, 0xb1, 0xd5, 0x4d, 0x54, 0x87, 0x8b, 0x4b, 0x95,
    0x1d, 0xcd, 0xf3, 0xfe, 0xa8, 0xda, 0xe0, 0xb6, 0x72, 0x13, 0x3f, 0x2e,
    0x66, 0xe0, 0xb9, 0x2e, 0xfa, 0x69, 0x40, 0xbe, 0xd7, 0x67, 0x6e, 0x53,
    0x2b, 0x3f, 0x53, 0xe5, 0x39, 0x54, 0x77, 0xe1, 0x1d, 0xe6, 0x81, 0x92,
    0x58, 0x82, 0x14, 0xfb, 0x47, 0x85, 0x3c, 0xc3, 0xdf, 0xdd, 0xcc, 0x79,
    0x9f, 0x41, 0x83, 0x72, 0xf2, 0x0a, 0xe9, 0xe1, 0x2c, 0x12, 0xb0, 0xb0,
    0x0a, 0xb2, 0x1d, 0xca, 0x15, 0xb2, 0xca,
};

/* kEmptyPasswordOpenSSL is a PKCS#8 encrypted private key using the empty
 * password and generated by OpenSSL. */
static const uint8_t kEmptyPasswordOpenSSL[] = {
    0x30, 0x82, 0x01, 0xa1, 0x30, 0x1b, 0x06, 0x0a, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x0c, 0x01, 0x03, 0x30, 0x0d, 0x04, 0x08, 0x86, 0xaa,
    0xd7, 0xdf, 0x3b, 0x91, 0x97, 0x60, 0x02, 0x01, 0x01, 0x04, 0x82, 0x01,
    0x80, 0xcb, 0x2a, 0x14, 0xaa, 0x4f, 0x38, 0x4c, 0xe1, 0x49, 0x00, 0xe2,
    0x1a, 0x3a, 0x75, 0x87, 0x7e, 0x3d, 0xea, 0x4d, 0x53, 0xd4, 0x46, 0x47,
    0x23, 0x8f, 0xa1, 0x72, 0x51, 0x92, 0x86, 0x8b, 0xeb, 0x53, 0xe6, 0x6a,
    0x0a, 0x6b, 0xb6, 0xa0, 0xdc, 0x0f, 0xdc, 0x20, 0xc3, 0x45, 0x85, 0xf1,
    0x95, 0x90, 0x5c, 0xf4, 0xfa, 0xee, 0x47, 0xaf, 0x35, 0xd0, 0xd0, 0xd3,
    0x14, 0xde, 0x0d, 0xca, 0x1b, 0xd3, 0xbb, 0x20, 0xec, 0x9d, 0x6a, 0xd4,
    0xc1, 0xce, 0x60, 0x81, 0xab, 0x0c, 0x72, 0x10, 0xfa, 0x28, 0x3c, 0xac,
    0x87, 0x7b, 0x82, 0x85, 0x00, 0xb8, 0x58, 0x9c, 0x07, 0xc4, 0x7d, 0xa9,
    0xc5, 0x94, 0x95, 0xf7, 0x23, 0x93, 0x3f, 0xed, 0xef, 0x92, 0x55, 0x25,
    0x74, 0xbb, 0xd3, 0xd1, 0x67, 0x3b, 0x3d, 0x5a, 0xfe, 0x84, 0xf8, 0x97,
    0x7d, 0x7c, 0x01, 0xc7, 0xd7, 0x0d, 0xf8, 0xc3, 0x6d, 0xd6, 0xf1, 0xaa,
    0x9d, 0x1f, 0x69, 0x97, 0x45, 0x06, 0xc4, 0x1c, 0x95, 0x3c, 0xe0, 0xef,
    0x11, 0xb2, 0xb3, 0x72, 0x91, 0x9e, 0x7d, 0x0f, 0x7f, 0xc8, 0xf6, 0x64,
    0x49, 0x5e, 0x3c, 0x53, 0x37, 0x79, 0x03, 0x1c, 0x3f, 0x29, 0x6c, 0x6b,
    0xea, 0x4c, 0x35, 0x9b, 0x6d, 0x1b, 0x59, 0x43, 0x4c, 0x14, 0x47, 0x2a,
    0x36, 0x39, 0x2a, 0xd8, 0x96, 0x90, 0xdc, 0xfc, 0xd2, 0xdd, 0x23, 0x0e,
    0x2c, 0xb3, 0x83, 0xf9, 0xf2, 0xe3, 0xe6, 0x99, 0x53, 0x57, 0x33, 0xc5,
    0x5f, 0xf9, 0xfd, 0x56, 0x0b, 0x32, 0xd4, 0xf3, 0x9d, 0x5b, 0x34, 0xe5,
    0x94, 0xbf, 0xb6, 0xc0, 0xce, 0xe1, 0x73, 0x5c, 0x02, 0x7a, 0x4c, 0xed,
    0xde, 0x23, 0x38, 0x89, 0x9f, 0xcd, 0x51, 0xf3, 0x90, 0x80, 0xd3, 0x4b,
    0x83, 0xd3, 0xee, 0xf2, 0x9e, 0x35, 0x91, 0xa5, 0xa3, 0xc0, 0x5c, 0xce,
    0xdb, 0xaa, 0x70, 0x1e, 0x1d, 0xc1, 0x44, 0xea, 0x3b, 0xa7, 0x5a, 0x11,
    0xd1, 0xf3, 0xf3, 0xd0, 0xf4, 0x5a, 0xc4, 0x99, 0xaf, 0x8d, 0xe2, 0xbc,
    0xa2, 0xb9, 0x3d, 0x86, 0x5e, 0xba, 0xa0, 0xdf, 0x78, 0x81, 0x7c, 0x54,
    0x31, 0xe3, 0x98, 0xb5, 0x46, 0xcb, 0x4d, 0x26, 0x4b, 0xf8, 0xac, 0x3a,
    0x54, 0x1b, 0x77, 0x5a, 0x18, 0xa5, 0x43, 0x0e, 0x14, 0xde, 0x7b, 0xb7,
    0x4e, 0x45, 0x99, 0x03, 0xd1, 0x3d, 0x18, 0xb2, 0x36, 0x00, 0x48, 0x07,
    0x72, 0xbb, 0x4f, 0x21, 0x25, 0x3e, 0xda, 0x25, 0x24, 0x5b, 0xc8, 0xa0,
    0x28, 0xd5, 0x9b, 0x96, 0x87, 0x07, 0x77, 0x84, 0xff, 0xd7, 0xac, 0x71,
    0xf6, 0x61, 0x63, 0x0b, 0xfb, 0x42, 0xfd, 0x52, 0xf4, 0xc4, 0x35, 0x0c,
    0xc2, 0xc1, 0x55, 0x22, 0x42, 0x2f, 0x13, 0x7d, 0x93, 0x27, 0xc8, 0x11,
    0x35, 0xc5, 0xe3, 0xc5, 0xaa, 0x15, 0x3c, 0xac, 0x30, 0xbc, 0x45, 0x16,
    0xed,
};

static bool TestDecrypt(const uint8_t *der, size_t der_len,
                        const char *password) {
  const uint8_t *data = der;
  bssl::UniquePtr<X509_SIG> sig(d2i_X509_SIG(NULL, &data, der_len));
  if (sig.get() == NULL || data != der + der_len) {
    fprintf(stderr, "d2i_X509_SIG failed or did not consume all bytes.\n");
    return false;
  }

  bssl::UniquePtr<PKCS8_PRIV_KEY_INFO> keypair(
      PKCS8_decrypt(sig.get(), password, -1));
  if (!keypair) {
    fprintf(stderr, "PKCS8_decrypt failed.\n");
    ERR_print_errors_fp(stderr);
    return false;
  }

  return true;
}

static bool TestRoundTrip(int pbe_nid, const EVP_CIPHER *cipher,
                          const char *password, const uint8_t *salt,
                          size_t salt_len, int iterations) {
  static const uint8_t kSampleKey[] = {
      0x30, 0x81, 0x87, 0x02, 0x01, 0x00, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86,
      0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
      0x03, 0x01, 0x07, 0x04, 0x6d, 0x30, 0x6b, 0x02, 0x01, 0x01, 0x04, 0x20,
      0x8a, 0x87, 0x2f, 0xb6, 0x28, 0x93, 0xc4, 0xd1, 0xff, 0xc5, 0xb9, 0xf0,
      0xf9, 0x17, 0x58, 0x06, 0x9f, 0x83, 0x52, 0xe0, 0x8f, 0xa0, 0x5a, 0x49,
      0xf8, 0xdb, 0x92, 0x6c, 0xb5, 0x72, 0x87, 0x25, 0xa1, 0x44, 0x03, 0x42,
      0x00, 0x04, 0x2c, 0x15, 0x0f, 0x42, 0x9c, 0xe7, 0x0f, 0x21, 0x6c, 0x25,
      0x2c, 0xf5, 0xe0, 0x62, 0xce, 0x1f, 0x63, 0x9c, 0xd5, 0xd1, 0x65, 0xc7,
      0xf8, 0x94, 0x24, 0x07, 0x2c, 0x27, 0x19, 0x7d, 0x78, 0xb3, 0x3b, 0x92,
      0x0e, 0x95, 0xcd, 0xb6, 0x64, 0xe9, 0x90, 0xdc, 0xf0, 0xcf, 0xea, 0x0d,
      0x94, 0xe2, 0xa8, 0xe6, 0xaf, 0x9d, 0x0e, 0x58, 0x05, 0x6e, 0x65, 0x31,
      0x04, 0x92, 0x5b, 0x9f, 0xe6, 0xc9,
  };

  const uint8_t *ptr = kSampleKey;
  bssl::UniquePtr<PKCS8_PRIV_KEY_INFO> key(
      d2i_PKCS8_PRIV_KEY_INFO(nullptr, &ptr, sizeof(kSampleKey)));
  if (!key || ptr != kSampleKey + sizeof(kSampleKey)) {
    return false;
  }

  bssl::UniquePtr<X509_SIG> encrypted(PKCS8_encrypt(
      pbe_nid, cipher, password, -1, salt, salt_len, iterations, key.get()));
  if (!encrypted) {
    fprintf(stderr, "Failed to encrypt private key.\n");
    return false;
  }

  bssl::UniquePtr<PKCS8_PRIV_KEY_INFO> key2(
      PKCS8_decrypt(encrypted.get(), password, -1));
  if (!key2) {
    fprintf(stderr, "Failed to decrypt private key.\n");
    return false;
  }

  uint8_t *encoded = nullptr;
  int len = i2d_PKCS8_PRIV_KEY_INFO(key2.get(), &encoded);
  bssl::UniquePtr<uint8_t> free_encoded(encoded);
  if (len < 0 ||
      static_cast<size_t>(len) != sizeof(kSampleKey) ||
      OPENSSL_memcmp(encoded, kSampleKey, sizeof(kSampleKey)) != 0) {
    fprintf(stderr, "Decrypted private key did not round-trip.");
    return false;
  }

  return true;
}

int main(int argc, char **argv) {
  CRYPTO_library_init();

  if (!TestDecrypt(kDER, sizeof(kDER), "testing") ||
      !TestDecrypt(kNullPassword, sizeof(kNullPassword), NULL) ||
      !TestDecrypt(kNullPasswordNSS, sizeof(kNullPasswordNSS), NULL) ||
      !TestDecrypt(kEmptyPasswordOpenSSL, sizeof(kEmptyPasswordOpenSSL), "") ||
      !TestRoundTrip(NID_pbe_WithSHA1And3_Key_TripleDES_CBC, nullptr,
                     "password", nullptr, 0, 10) ||
      // Vary the salt
      !TestRoundTrip(NID_pbe_WithSHA1And3_Key_TripleDES_CBC, nullptr,
                     "password", nullptr, 4, 10) ||
      !TestRoundTrip(NID_pbe_WithSHA1And3_Key_TripleDES_CBC, nullptr,
                     "password", (const uint8_t *)"salt", 4, 10) ||
      // Vary the iteration count.
      !TestRoundTrip(NID_pbe_WithSHA1And3_Key_TripleDES_CBC, nullptr,
                     "password", nullptr, 0, 1) ||
      // Vary the password.
      !TestRoundTrip(NID_pbe_WithSHA1And3_Key_TripleDES_CBC, nullptr, "",
                     nullptr, 0, 1) ||
      !TestRoundTrip(NID_pbe_WithSHA1And3_Key_TripleDES_CBC, nullptr, nullptr,
                     nullptr, 0, 1) ||
      // Vary the PBE suite.
      !TestRoundTrip(NID_pbe_WithSHA1And40BitRC2_CBC, nullptr, "password",
                     nullptr, 0, 10) ||
      !TestRoundTrip(NID_pbe_WithSHA1And128BitRC4, nullptr, "password", nullptr,
                     0, 10) ||
      // Test PBES2.
      !TestRoundTrip(-1, EVP_aes_128_cbc(), "password", nullptr, 0, 10) ||
      !TestRoundTrip(-1, EVP_aes_128_cbc(), "password", nullptr, 4, 10) ||
      !TestRoundTrip(-1, EVP_aes_128_cbc(), "password", (const uint8_t *)"salt",
                     4, 10) ||
      !TestRoundTrip(-1, EVP_aes_128_cbc(), "password", nullptr, 0, 1) ||
      !TestRoundTrip(-1, EVP_rc2_cbc(), "password", nullptr, 0, 10)) {
    return 1;
  }

  printf("PASS\n");
  return 0;
}
