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

typedef read_result_t (*reader_t)(slice_t *slice, message_base_t base, result_t *result);

#define READER_TYPE_ALIAS(name, type, free) \
    static inline read_result_ok_t *name##_read_result_ok_new(read_result_base_t base, type *data, size_t len) { return read_result_ok_new(base, free, data, len); } \
    static inline type* name##_read_result_ok_extract(read_result_ok_t *ok) { return (type*)ok->data; } \
    typedef read_result_ok_t name##_read_result_ok_t; \
    typedef read_result_t name##_read_result_t;

#define PUSH_ERROR(err) result_push(result, (message_t)message_error_new(base, err))
#define NOT_THIS() read_result_base_new(READ_NOT_THIS)
#define INVALID(len) (read_result_t)read_result_invalid_new(read_result_base_new_simple(), len)

#define EXPECT_SCOPE_OR_NOT_THIS(err) do { if (err) { if (err == TOKEN_GET_SCOPE_NOT_STARTED) { return NOT_THIS(); } else if (err == TOKEN_GET_SCOPE_NOT_CLOSED) { PUSH_ERROR(ERROR_SCOPE_NOT_CLOSED); return INVALID(tokens->len); } } } while(0)

// Constructor
#define READING_SETUP size_t i = 0
#define READING_SETUP_FINISH i = slice.len - cur_slice.len

#define READING_LOOP while (i < slice.len)

#define READING_ITER_SETUP bool found = false, invalid_last_time = false; str_slice_t cur_slice = subslice_after(&slice, i)
#define READING_ITER_FINISH(not_found_error) if (!found) { if (!invalid_last_time) { PUSH_ERROR(not_found_error); invalid_last_time = true; } } else { invalid_last_time = false; }
#define READER_CHANGE_POS_ON_NOT_FOUND if (!found) 

#define READERS_LOOP(count) for (size_t rid = 0; rid < count; rid++)
#define READERS_TRY_READ(_readers) reader_t *readers = _readers; read_result_t read_result = readers[rid](&cur_slice, base, result); \
	if (read_result->kind == READ_ERR_INVALID) { \
		read_result_invalid_t *error = read_result_as_invalid(read_result); \
		i += error->len; \
		found = true; \
	} else if (read_result->kind == READ_OK) { \
		read_result_ok_t *ok = read_result_as_ok(read_result); \
		i += ok->len; \
		found = true; \
	} if (read_result->kind == READ_OK) 
#define READERS_READ_END read_result_free(read_result); if (found) break