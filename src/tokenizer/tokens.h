#pragma once

#include <stddef.h>
#include <stdint.h>
#include "core/str.h"
#include "core/sealed.h"
#include "base/location.h"

SEALED_KIND(token,
    // PUNCTUATION
    TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_COLON,
    // DEF
    TOKEN_CONST, TOKEN_VAR, TOKEN_FUN, TOKEN_SET,
    // FUN
    TOKEN_RETURN,
    // CONDITIONS
    TOKEN_IF, TOKEN_ELIF, TOKEN_ELSE,
    // BRACES
	TOKEN_OPENING_FIGURE_BRACE, TOKEN_CLOSING_FIGURE_BRACE,
	TOKEN_OPENING_CIRCLE_BRACE, TOKEN_CLOSING_CIRCLE_BRACE,
    // OPERATIONS
	TOKEN_EQUALS, TOKEN_NOT_EQUALS, TOKEN_GE, TOKEN_LE, TOKEN_GREATER, TOKEN_LESS,
    TOKEN_DIVIDE, TOKEN_MULTIPLY, TOKEN_MINUS, TOKEN_PLUS, TOKEN_MOD,
    // INTERHITED
    TOKEN_IDENT,
    TOKEN_UINT, TOKEN_STR
);

SEALED_BASE(token,
    (location_t, location)
);

SEALED_CHILD_DEFINE(token, TOKEN_IDENT, ident,
    (str_t, ident)
);

SEALED_CHILD_DEFINE_FREE(token, TOKEN_UINT, uint,
    (uint64_t, value)
);

SEALED_CHILD_DEFINE_FREE(token, TOKEN_STR, str,
    (str_t, value)
);

SEALED_FREE_DEFINE(token);

ARR_TYPED_ALIAS_DEFINE(tokens, token_t);

typedef enum {
    TOKEN_GET_SCOPE_OK = 0,
    TOKEN_GET_SCOPE_NOT_STARTED,
    TOKEN_GET_SCOPE_NOT_CLOSED,
} token_get_scope_err_t;

token_get_scope_err_t token_try_get_scope(tokens_slice_t *slice, tokens_slice_t *dst, token_type_t opening, token_type_t closing);
tokens_slice_t tokens_before(tokens_slice_t *slice, token_type_t type);
tokens_slice_t tokens_before_scoped(tokens_slice_t *slice, token_type_t type, token_type_t opening, token_type_t closing);
#define token_get_circle_scope(slice, dst) token_try_get_scope(slice, dst, TOKEN_OPENING_CIRCLE_BRACE, TOKEN_CLOSING_CIRCLE_BRACE)
#define token_get_figure_scope(slice, dst) token_try_get_scope(slice, dst, TOKEN_OPENING_FIGURE_BRACE, TOKEN_CLOSING_FIGURE_BRACE)
#define tokens_before_circle_scoped(slice, type) tokens_before_scoped(slice, type, TOKEN_OPENING_CIRCLE_BRACE, TOKEN_CLOSING_CIRCLE_BRACE)