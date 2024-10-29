#pragma once

#include "lexer/projects/modules/scopes/exprs/expr.h"
#include "../stat.h"

typedef struct {
    stat_base_t base;
    expr_t expr; // OPTIONAL
} stat_return_t;