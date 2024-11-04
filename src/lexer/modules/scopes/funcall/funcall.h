#pragma once

#include "core/arr.h"
#include "core/str.h"
#include "../exprs/expr_base.h"

ARR_TYPED_ALIAS_DEFINE(funcall_args, expr_t);

typedef struct {
    str_t ident;
    funcall_args_t args;
} funcall_t;

void funcall_free(funcall_t *funcall);

