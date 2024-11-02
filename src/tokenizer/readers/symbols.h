#pragma once

#include "chars.h"
#include "reader.h"

#define DEFINE_SYMBOL(name, token_type, text) \
    token_read_result_t symbol_##name##_reader(str_slice_t *slice) { \
        static token_type_t keyword_token_type = token_type; \
        static const char *keyword_cstr = text; \
        str_slice_t keyword = str_slice_from_cstr(keyword_cstr); \
        return str_slice_starts_with(slice, &keyword) ? (token_read_result_t)token_read_result_ok_new(  \
            token_read_result_base_new_simple(),  \
            token_base_new(keyword_token_type, location_new(0, 0)),  \
            keyword.len,  \
            false,  \
            warning_variants_count  \
        ) : (token_read_result_t)token_read_result_base_new(TOKEN_READ_NOT_THIS);  \
    }