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

// BINOP

typedef enum {
	BINOP_PLUS,
	BINOP_MINUS,
	BINOP_MULTIPLY,
	BINOP_DIVIDE,

	BINOP_EQ,
	BINOP_NEQ,
	BINOP_GE,
	BINOP_LE,

	BINOP_GREATER,
	BINOP_LESS,
} binop_type_t;

typedef struct {
	expr_type_t type = EXPR_BINOP;
	binop_type_t binop_type;
	expr_t first, second;
} expr_binop_t;

// FUNCALL

typedef arr_t fun_args_t;

typedef struct {
	expr_type_t type = EXPR_BINOP;
	ident_t name;
	fun_args_t args;
} expr_funcall_t;

// IDENT

typedef struct {
	expr_type_t type = EXPR_FUNC;
	ident_t name;
} expr_ident_t;

// -----

typedef struct {
	ast_node_type_t type = AST_NODE_EXPR;
	ident_t result_type;
} ast_node_expr_t;

