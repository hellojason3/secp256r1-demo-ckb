#ifndef DAS_DEMO_SECP256R1_UTILS_H
#define DAS_DEMO_SECP256R1_UTILS_H
#include "deps/ckb-c-stdlib/libc/entry.h"

#ifdef CKB_C_STDLIB_PRINTF
#define debug_print(s)							ckb_debug(s)
#define debug_print_int(prefix, value)			debug_print_int_impl((prefix), (value))
#define debug_print_data(prefix, data, data_len)  debug_print_data_impl((prefix), (data), (data_len))
#define debug_print_string(prefix, data, data_len)  debug_print_string_impl((prefix), (data), (data_len))

static char debug_buffer[64 * 1024];
static void debug_print_data_impl(const char *prefix,
                                  const uint8_t *data,
                                  uint32_t data_len) {
    int offset = 0;
    offset += sprintf_(debug_buffer, "%s", prefix);
    for (size_t i = 0; i < data_len; i++) {
        offset += sprintf_(debug_buffer + offset, "%02x", data[i]);
    }
    debug_buffer[offset] = '\0';
    ckb_debug(debug_buffer);
}
static void debug_print_int_impl(const char *prefix, int ret) {
    int offset = 0;
    offset += sprintf_(debug_buffer, "%s%d", prefix, ret);
    debug_buffer[offset] = '\0';
    ckb_debug(debug_buffer);
}
static void debug_print_string_impl(const char *prefix,
                                  const uint8_t *data,
                                  uint32_t data_len) {
    int offset = 0;
    offset += sprintf_(debug_buffer, "%s", prefix);
    for (size_t i = 0; i < data_len; i++) {
        offset += sprintf_(debug_buffer + offset, "%c", data[i]);
    }
    debug_buffer[offset] = '\0';
    ckb_debug(debug_buffer);
}
#else
#define debug_print(s)
#define debug_print_int(prefix, value)
#define debug_print_data(prefix, data, data_len)
#define debug_print_string(prefix, data, data_len)
#endif


int convertHexCharToInt(char c){
    int t;
    if(c>='a' && c<='f'){
        t = c-'a' + 10;
    }else if (c>='0'&&c<='9'){
        t = c-'0';
    }else{
        t = 0;
    }
    return t;
}
void str2bin(unsigned char* dest, char* source, size_t input_len) {
    for (int i = 0; i < input_len; i++) {
        dest[i] = (convertHexCharToInt(source[i * 2]) << 4) + convertHexCharToInt(source[i * 2 + 1]);
    }
    return;
}



#endif //DAS_DEMO_SECP256R1_UTILS_H
