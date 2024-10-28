#pragma once

#include "core/arr.h"
#include "lexer/base.h"

typedef enum {
       EXPR_BINOP,
       EXPR_FUNCALL,
       EXPR_IDENT
} expr_type_t;

typedef struct {
	expr_type_t type;
} expr_base_t;

typedef expr_base_t* expr_t;

typedef struct {
	ast_node_type_t type = AST_NODE_EXPR;
	ident_t result_type;
} ast_node_expr_t;

