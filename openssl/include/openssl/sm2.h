/* crypto/sm2/sm2.h */
/* ====================================================================
 * Copyright (c) 2015 The GmSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the GmSSL Project.
 *    (http://gmssl.org/)"
 *
 * 4. The name "GmSSL Project" must not be used to endorse or promote
 *    products derived from this software without prior written
 *    permission. For written permission, please contact
 *    guanzhi1980@gmail.com.
 *
 * 5. Products derived from this software may not be called "GmSSL"
 *    nor may "GmSSL" appear in their names without prior written
 *    permission of the GmSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the GmSSL Project
 *    (http://gmssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE GmSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE GmSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 */


#ifndef HEADER_SM2_H
#define HEADER_SM2_H

#include <openssl/ec.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/asn1.h>
#include <openssl/ecdsa.h>

#ifdef __cplusplus
extern "C" {
#endif


#define SM2_MAX_ID_BITS				65535
#define SM2_MAX_ID_LENGTH			8191
#define SM2_DEFAULT_POINT_CONVERSION_FORM	POINT_CONVERSION_UNCOMPRESSED


typedef struct sm2_ciphertext_value_st {
	EC_POINT *ephem_point;    //65: 密文
	unsigned char *ciphertext;    //48: 异或
	size_t ciphertext_size;    //48
	unsigned char mactag[EVP_MAX_MD_SIZE];   //32: 哈希
	unsigned int mactag_size;    //32
} SM2_CIPHERTEXT_VALUE;


char *SM2_get_id(EC_KEY *ec_key);
int SM2_set_id(EC_KEY *ec_key, const char *id);
int SM2_compute_id_digest(unsigned char *dgst, unsigned int *dgstlen,
	const EVP_MD *md, const void *id, size_t idlen, EC_KEY *ec_key);

int SM2_CIPHERTEXT_VALUE_size(const EC_GROUP *ec_group,
	point_conversion_form_t point_form, size_t mlen,
	const EVP_MD *mac_md);
void SM2_CIPHERTEXT_VALUE_free(SM2_CIPHERTEXT_VALUE *cv);
int SM2_CIPHERTEXT_VALUE_encode(const SM2_CIPHERTEXT_VALUE *cv,
	const EC_GROUP *ec_group, point_conversion_form_t point_form,
	unsigned char *buf, size_t *buflen);
SM2_CIPHERTEXT_VALUE *SM2_CIPHERTEXT_VALUE_decode(const EC_GROUP *ec_group,
	point_conversion_form_t point_form, const EVP_MD *mac_md,
	const unsigned char *buf, size_t buflen);
int SM2_CIPHERTEXT_VALUE_print(BIO *out, const EC_GROUP *ec_group,
	const SM2_CIPHERTEXT_VALUE *cv, int indent, unsigned long flags);
SM2_CIPHERTEXT_VALUE *SM2_do_encrypt(const EVP_MD *kdf_md, const EVP_MD *mac_md,
	const unsigned char *in, size_t inlen, EC_KEY *ec_key);
int SM2_do_decrypt(const EVP_MD *kdf_md, const EVP_MD *mac_md,
	const SM2_CIPHERTEXT_VALUE *cv, unsigned char *out, size_t *outlen,
	EC_KEY *ec_key);
int SM2_encrypt(const EVP_MD *kdf_md, const EVP_MD *mac_md,
	point_conversion_form_t point_form,
	const unsigned char *in, size_t inlen,
	unsigned char *out, size_t *outlen, EC_KEY *ec_key);
int SM2_decrypt(const EVP_MD *kdf_md, const EVP_MD *mac_md,
	point_conversion_form_t point_form,
	const unsigned char *in, size_t inlen,
	unsigned char *out, size_t *outlen, EC_KEY *ec_key);

int sm2encrypt_encode_q1 (unsigned char *input, const int input_length, unsigned char *output);

int sm2decrypt_decode_q1 (unsigned char *q1ciphertext, unsigned char *ciphertext);

#define SM2_signature_size(ec_key)	ECDSA_size(ec_key)
int SM2_sign_setup(EC_KEY *ec_key, BN_CTX *ctx, BIGNUM **a, BIGNUM **b);
ECDSA_SIG *SM2_do_sign_ex(const unsigned char *dgst, int dgstlen,
	const BIGNUM *a, const BIGNUM *b, EC_KEY *ec_key);
ECDSA_SIG *SM2_do_sign(const unsigned char *dgst, int dgst_len,
	EC_KEY *ec_key);
int SM2_do_verify(const unsigned char *dgst, int dgstlen,
	const ECDSA_SIG *sig, EC_KEY *ec_key);
int SM2_sign_ex(int type, const unsigned char *dgst, int dgstlen,
	unsigned char *sig, unsigned int *siglen,
	const BIGNUM *k, const BIGNUM *x, EC_KEY *ec_key);
int SM2_sign(int type, const unsigned char *dgst, int dgstlen,
	unsigned char *sig, unsigned int *siglen, EC_KEY *eckey);
int SM2_verify(int type, const unsigned char *dgst, int dgstlen,
	const unsigned char *sig, int siglen, EC_KEY *ec_key);

int SM2_compute_key(SSL *s, const EC_KEY *ecdh, const EC_POINT *point, unsigned char *out);

void print_bn(BIGNUM *bn);

void print_point(const EC_GROUP *group, EC_POINT *point);

// unsigned char pubkey[64+1];
int get_pubkey_from_ec_key(EC_KEY *ec_key, unsigned char *out, unsigned int len);

// unsigned char prikey[32];
int get_prikey_from_ec_key(EC_KEY *ec_key, unsigned char *out);

int get_id(const char *id, int id_len, unsigned char *pubkey, int pubkey_len, unsigned char md[32]);

void ERR_load_SM2_strings(void);

/* Function codes. */
#define SM2_F_SM2_SET_ID			100
#define SM2_F_SM2_GET_ID			101
#define SM2_F_SM2_COMPUTE_ID_DIGEST		102
#define SM2_F_SM2_CIPHERTEXT_VALUE_SIZE		103
#define SM2_F_SM2_CIPHERTEXT_VALUE_FREE		104
#define SM2_F_SM2_CIPHERTEXT_VALUE_ENCODE	105
#define SM2_F_SM2_CIPHERTEXT_VALUE_DECODE	106
#define SM2_F_SM2_CIPHERTEXT_VALUE_PRINT	107
#define SM2_F_SM2_DO_ENCRYPT			108
#define SM2_F_SM2_DO_DECRYPT			109
#define SM2_F_SM2_ENCRYPT			110
#define SM2_F_SM2_DECRYPT			111
#define SM2_SIGNATURE_SIZE			112
#define SM2_SIGN_SETUP				113
#define SM2_DO_SIGN_EX				114
#define SM2_DO_SIGN				115
#define SM2_DO_VERIFY				116
#define SM2_SIGN_EX				117
#define SM2_SIGN				118
#define SM2_VERIFY				119

/* Reason codes. */
#define SM2_R_BAD_DATA				100
#define SM2_R_UNKNOWN_CIPHER_TYPE		101
#define SM2_R_ENCRYPT_FAILED			102
#define SM2_R_DECRYPT_FAILED			103
#define SM2_R_UNKNOWN_MAC_TYPE			104
#define SM2_R_GEN_MAC_FAILED			105
#define SM2_R_VERIFY_MAC_FAILED			106
#define SM2_R_ECDH_FAILED			107
#define SM2_R_BUFFER_TOO_SMALL			108

#ifdef __cplusplus
}
#endif
#endif

