#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "tokenizer/tokens.h"
#include "core/sealed.h"
#include "reader.h"

#define READER_SETUP size_t len = 0

#define EXPECT_TOKEN(token, type) \
    do { \
        if ((token)->kind != type) { \
            PUSH_ERROR(ERROR_INVALID_TOKEN); \
            return INVALID(tokens->len); \
        } \
    } while (0)

#define EXPECT_TOKEN_OR_NOT_THIS(token, type) \
    do { \
        if ((token)->kind != type) { \
            return NOT_THIS(); \
        } \
    } while (0)

#define EXPECT_TOKENS_COUNT(count) \
    do { \
        if (tokens->len < (count)) { \
            PUSH_ERROR(ERROR_MISSING_TOKEN); \
            return INVALID(tokens->len); \
        } \
    } while (0)

#define EXPECT_TOKENS_COUNT_OR_NOT_THIS(count) \
    do { \
        if (tokens->len < (count)) { \
            return NOT_THIS(); \
        } \
    } while (0)

token_t token_next(size_t *len, tokens_slice_t *tokens, message_base_t *base);

#define NEXT_TOKEN() token_next(&len, tokens, &base)