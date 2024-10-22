#pragma once

#include "lexer/base.h"

typedef struct {
	ident_t name;
	ast_node_block_t ast_block;
} func_t;

typedef arr_t funcs_t;
