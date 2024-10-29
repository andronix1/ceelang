#pragma once

#include "../stat.h"
#include "lexer/projects/modules/scopes/exprs/funcall/funcall.h"

typedef struct {
    stat_base_t base;
    expr_t cond;
    stats_t if_stats;
    stats_t *else_stats; // OPTIONAL
} stat_if_t;