#pragma once

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "core/arr.h"
#include "core/str.h"
#include "char_ext.h"

#define TOKENS_CAPACITY 16

typedef enum {
	// META
	TOKEN_SEMICOLON = 0,
	// WITHOUT ARGS
	TOKEN_CONST,
	TOKEN_VAR,
	TOKEN_RETURN,
	TOKEN_FUN,
	TOKEN_IF, TOKEN_ELSE,
	TOKEN_OPENING_FIGURE_BRACE, TOKEN_CLOSING_FIGURE_BRACE,
	TOKEN_OPENING_CIRCLE_BRACE, TOKEN_CLOSING_CIRCLE_BRACE,
	TOKEN_COMMA,
	TOKEN_COLON,
	//operations
	TOKEN_EQUALS, TOKEN_NOT_EQUALS, TOKEN_GE, TOKEN_LE, TOKEN_GREATER, TOKEN_LESS, TOKEN_SET,
	TOKEN_MOD, TOKEN_DIVIDE, TOKEN_MULTIPLY, TOKEN_MINUS, TOKEN_PLUS,

	// WITH ARGS
	TOKEN_IDENT,
	TOKEN_UINT,
	TOKEN_STR,

	// INTERNAL
	TOKENS_COUNT,
} token_type_t;

typedef struct {
	token_type_t type;
} token_base_t;

typedef struct {
	token_type_t type;
	str_t ident;
} token_ident_t;

typedef struct {
	token_type_t type;
	uint64_t value;
} token_uint_t;

typedef struct {
	token_type_t type;
	str_t value;
} token_str_t;

typedef token_base_t* token_t;

typedef arr_t tokens_t;
typedef slice_t tokens_slice_t;

extern const char *str_token[TOKENS_COUNT];

void token_expect_type(token_t token, token_type_t token_type);
token_t tokens_try_get(tokens_slice_t *slice, size_t idx);
tokens_slice_t tokens_get_circle_scope(tokens_slice_t *slice, size_t from);
tokens_slice_t tokens_get_figure_scope(tokens_slice_t *slice, size_t from);
tokens_slice_t tokens_before(tokens_slice_t *slice, token_type_t type);
tokens_slice_t tokens_before_figure_scoped(tokens_slice_t *slice, token_type_t type);
tokens_slice_t tokens_before_circle_scoped_included(tokens_slice_t *slice, token_type_t type);
tokens_slice_t tokens_before_circle_scoped(tokens_slice_t *slice, token_type_t type);
str_t token_extract_ident(token_t token);