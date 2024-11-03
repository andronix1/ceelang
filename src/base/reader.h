#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "tokenizer/tokens.h"
#include "core/sealed.h"
#include "base/result.h"

SEALED_KIND(read_result,
    READ_OK,
    READ_NOT_THIS,
    READ_ERR_INVALID
);

SEALED_BASE(read_result);

typedef void(*free_func_t)(void*);

SEALED_CHILD_DEFINE_FREE(read_result, READ_OK, ok,
    SEALED_PARAM(free_func_t, free),
    SEALED_PARAM(void*, data),
    SEALED_PARAM(size_t, len)
);

SEALED_CHILD_DEFINE(read_result, READ_ERR_INVALID, invalid,
    SEALED_PARAM(size_t, len)
);

SEALED_FREE_DEFINE(read_result);

#define READER_TYPE_ALIAS(name, type, free) \
    static inline read_result_ok_t *name##_read_result_ok_new(read_result_base_t base, type *data, size_t len) { return read_result_ok_new(base, free, data, len); } \
    static inline type* name##_read_result_ok_extract(read_result_ok_t *ok) { return (type*)ok->data; } \
    typedef read_result_ok_t name##_read_result_ok_t; \
    typedef read_result_t name##_read_result_t;
