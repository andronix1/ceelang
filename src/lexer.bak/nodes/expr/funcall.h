#pragma once

typedef arr_t fun_args_t;

typedef struct {
	expr_type_t type = EXPR_BINOP;
	ident_t name;
	fun_args_t args;
} expr_funcall_t;


