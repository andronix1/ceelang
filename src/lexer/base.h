#pragma once

#include "core/str.h"

typedef str_t ident_t;

typedef enum {
	AST_NODE_BLOCK,
	AST_NODE_EXPR,
} ast_node_type_t;

typedef struct {
	ast_node_type_t type;
} ast_node_base_t;

typedef arr_t ast_nodes_t;
