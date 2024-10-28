#pragma once

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

