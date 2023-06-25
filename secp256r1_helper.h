#ifndef DAS_DEMO_SECP256R1_SECP256R1_HELPER_H
#define DAS_DEMO_SECP256R1_SECP256R1_HELPER_H

//#include "deps/libecc/src/lib_ecc_types.h"
//#include "deps/libecc/src/libec.h"
//#include "deps/libecc/src/libsig.h"

#include "lib_ecc_types.h"
#include "libec.h"
#include "libsig.h"

typedef struct {
    ec_alg_type sig_algo;
    hash_alg_type hash_algo;
    ec_params ec_params;
} secp256r1_context_t;

int secp256r1_context_init(secp256r1_context_t *context) {
    context->sig_algo = ECDSA;
    context->hash_algo = SHA256;
    int ret = import_params(&context->ec_params, &secp256r1_str_params);
    return ret;
}

int secp256r1_recover_public_key_from_signature(const secp256r1_context_t *context,
                                                ec_pub_key *pub_key1,
                                                ec_pub_key *pub_key2, const u8 *sig,
                                                u8 siglen, const u8 *hash,
                                                u8 hsize) {
    return __ecdsa_public_key_from_sig(pub_key1, pub_key2, &context->ec_params,
                                       sig, siglen, hash, hsize,
                                       context->sig_algo);
}

int secp256r1_pub_key_export_to_aff_buf(const secp256r1_context_t *context,
                                        const ec_pub_key *pub_key, u8 *pub_key_buf,
                                        u8 pub_key_buf_len) {
    return ec_pub_key_export_to_aff_buf(pub_key, pub_key_buf, pub_key_buf_len);
};

int secp256r1_verify_signature(const secp256r1_context_t *context, const u8 *sig,
                               u8 siglen, const ec_pub_key *pub_key, const u8 *m,
                               u32 mlen) {
    int ret;
    ret = ec_verify(sig, siglen, pub_key, m, mlen, context->sig_algo,
                    context->hash_algo, NULL, 0);
    debug_print_int("signature verification result: ", ret);
    return ret;
}

#endif //DAS_DEMO_SECP256R1_SECP256R1_HELPER_H
