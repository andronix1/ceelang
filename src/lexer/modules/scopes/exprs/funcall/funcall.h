#pragma once

#include "core/str.h"
#include "../expr.h"

typedef arr_t funcall_args_t;

typedef struct {
    str_t ident;
    funcall_args_t args;
} funcall_t;

typedef struct {
    expr_base_t base;
    funcall_t funcall;
} expr_funcall_t;

typedef expr_t funcall_arg_t;