#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "tokenizer/tokens.h"
#include "core/sealed.h"
#include "base/reader.h"
#include "def.h"
#include "base/result.h"

READER_TYPE_ALIAS(def, def_t, free);

typedef def_read_result_t (*def_reader_t)(tokens_slice_t *tokens, message_base_t base, result_t *result);

// Reader utils

#define PUSH_ERROR(err) result_push(result, (message_t)message_error_new(base, err))
#define INVALID(len) (def_read_result_t)read_result_invalid_new(read_result_base_new_simple(), len)

#define EXPECT_TOKEN(token, type) \
    do { \
        if ((token)->kind != type) { \
            PUSH_ERROR(ERROR_INVALID_TOKEN); \
            return INVALID(tokens->len); \
        } \
    } while (0)

#define EXPECT_TOKENS_COUNT(count) \
    do { \
        if (tokens->len < (count)) { \
            PUSH_ERROR(ERROR_MISSING_TOKEN); \
            return INVALID(tokens->len); \
        } \
    } while (0)

token_t token_next(size_t *len, tokens_slice_t *tokens, message_base_t *base);

#define NEXT_TOKEN() token_next(&len, tokens, &base)