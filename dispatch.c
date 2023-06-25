#include "deps/ckb-c-stdlib/ckb_dlfcn.h"
#include "deps/ckb-c-stdlib/ckb_syscalls.h"
#include "utils.h"
#include "secp256r1_helper.h"
typedef unsigned char u8;

void print_pub_key(u8* pubkey, const char* title) {
    debug_print_data(title, pubkey, 64);
}

void convert_aff_buf_to_prj_buf(const u8 *aff_buf, u32 aff_buf_len, u8 *prj_buf,
                                u32 prj_buf_len) {
    static const u8 z_buf[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    //MUST_HAVE(aff_buf_len == affine_buffer_size);
   // MUST_HAVE(prj_buf_len == projective_buffer_size);
    memcpy(prj_buf, aff_buf, aff_buf_len);
    memcpy(prj_buf + aff_buf_len, z_buf, sizeof(z_buf));
}


int main() {
    int ret = 0;
    //print hello
    u8 data[10] = "hello test";
    debug_print_string("test print = ", data, 10);

    //init context
    secp256r1_context_t context;
    ret = secp256r1_context_init(&context);
    debug_print_int("init context result: ", ret);

    //import signature
    char* sig_str = "5f8ac7174a08a40829f40b0c57c7995157e0b992450132d761a94507089ef54313c371551a476e0b46b04723273da0653ddc602b0bb78d2745a86629d66491e4";
    u8 sig[64] = {0};
    str2bin(sig, sig_str, 64);
    //import message
    char* msg_str = "093803cb756c07b9c82f5ce82be28d2149d641ce0f245fa91a0ea84145f78803";
    u8 msg[32] = {0};
    str2bin(msg, msg_str, 32);

    //recover pubkey
    ec_pub_key pk1, pk2;
    ret = 0;
    ret = secp256r1_recover_public_key_from_signature(&context, &pk1, &pk2, sig, 64, msg, 32);
    debug_print_int("Recover PK from signature, ret ", ret);

    //convert pubkey to vec
    u8 pubkey1[64] = {0};
    ret = secp256r1_pub_key_export_to_aff_buf(&context, &pk1, pubkey1, 64);
    debug_print_int("PK1 export to buf, ret ", ret);
    //expected pk1 dad134c166477c7406dead9a4eb9e663853e0e61881a53ab44a9f8dc9a6a0c522dfaef1f5ef16cbb7501e92b6a669e8debaac040cf92ff3b981ef81807746d76
    print_pub_key(pubkey1, "PK1 = ");

    u8 pubkey2[64] = {0};
    ret = secp256r1_pub_key_export_to_aff_buf(&context, &pk2, pubkey2, 64);
    debug_print_int("PK2 export to buf, ret ", ret);
    //expected pk2 3edc4f6d1ba31cae2f8e7af0b6d82d79c695576c837df0105bf4d1d785831c556dd11a9ccb13ddafa8c9c978c9a98b4e74579956ffd36b2a00f09f858e8a2224
    print_pub_key(pubkey2, "PK2 = ");

    //verify signature
    char* msg_before_sha256 = "49960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000e8e4d0f5939b2ddb8bccd93ad010a28d9d9e6bd8a350d43565dcd5dc1d897a6e";
    u8 msg_before_sha256_bin[69] = {0};
    str2bin(msg_before_sha256_bin, msg_before_sha256, 69);
    debug_print_data("msg_before_sha256_bin = ", msg_before_sha256_bin, 123);
    u8 pj_pk_buf[96];
    convert_aff_buf_to_prj_buf(pubkey2, 64, pj_pk_buf, 96);

    ec_pub_key pub_key;
    ret = ec_pub_key_import_from_buf(&pub_key, &context.ec_params, pj_pk_buf, 96, context.sig_algo);
    debug_print_int("ec_pub_key_import_from_buf, ret ", ret);

    ret = secp256r1_verify_signature(&context, sig, 64, &pub_key, msg_before_sha256_bin, 69);
    debug_print_int("secp256r1_verify_signature, ret ", ret);

    return 0;
}
