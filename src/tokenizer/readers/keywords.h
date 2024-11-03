#pragma once

#include "chars.h"
#include "reader.h"

#define DEFINE_KEYWORD(name, token_type, text) \
    token_read_result_t keyword_##name##_reader(str_slice_t *slice, message_base_t base, result_t *result) { \
        static token_type_t keyword_token_type = token_type; \
        static const char *keyword_cstr = text; \
        str_slice_t keyword = str_slice_from_cstr(keyword_cstr); \
        return (str_slice_starts_with(slice, &keyword) && (  \
            slice->len <= keyword.len ||   \
            !is_ident(*str_slice_at(slice, keyword.len))  \
        )) ? (token_read_result_t)token_read_result_ok_new(  \
            read_result_base_new_simple(),  \
            token_base_new(keyword_token_type, location_new(0, 0)),  \
            keyword.len  \
        ) : (token_read_result_t)read_result_base_new(READ_NOT_THIS);  \
    }
