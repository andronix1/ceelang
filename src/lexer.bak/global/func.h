#pragma once

#include "lexer/base.h"
#include "lexer/nodes/code/node.h"

typedef struct {
	ident_t name;
	code_node_t body;
} func_t;

typedef arr_t funcs_t;
