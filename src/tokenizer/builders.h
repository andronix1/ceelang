#pragma once

#include "tokens.h"

typedef enum {
	TOKEN_BUILD_OK = 0,
	TOKEN_BUILD_ERR_IS_NOT_THIS,
	TOKEN_BUILD_ERR_INVALID
} token_build_err_t;

typedef struct {
	token_build_err_t err;
	token_t token;
	size_t end;
} token_build_result_t;

typedef token_build_result_t(*token_builder_t)(str_slice_t*);

extern token_build_result_t token_build_is_not_this;

#define token_build_ok_init(dst, type, len, setup) do {	\
		dst.err = TOKEN_BUILD_OK; 		\
		dst.token = malloc(sizeof(type)); 	\
		dst.end = len; 				\
		type *token = (type*)result.token;	\
		setup					\
	} while (0)
	
