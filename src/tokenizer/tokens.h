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

typedef token_base_t* token_t;

typedef arr_t tokens_t;

