#pragma once

#include "lexer/modules/scopes/exprs/expr.h"
#include "core/arr.h"
#include "core/sealed.h"

SEALED_KIND(stat,
    STAT_DEFINE,
    STAT_FUNCALL,
    STAT_RETURN,
    STAT_IF
);

SEALED_BASE(stat);

ARR_TYPED_ALIAS_DEFINE(stats, stat_t);

typedef enum {
    STAT_DEFINE_CONST,
    STAT_DEFINE_VAR,
} stat_define_type_t;

SEALED_CHILD_DEFINE_FREE(stat, STAT_DEFINE, define,
    (stat_define_type_t, define_type),
    (str_t, name),
    (str_t, type),
    (expr_t, expr)
);

SEALED_CHILD_DEFINE_FREE(stat, STAT_FUNCALL, funcall,
    (funcall_t*, funcall)
);

SEALED_CHILD_DEFINE_FREE(stat, STAT_RETURN, return,
    (expr_t, expr)
);

typedef struct {
    expr_t cond;
    stats_t stats;
} if_cond_stat_t;

void if_cond_stat_free(if_cond_stat_t *stat);

ARR_TYPED_ALIAS_DEFINE(elif_cond_stats, if_cond_stat_t);

SEALED_CHILD_DEFINE_FREE(stat, STAT_IF, if,
    (if_cond_stat_t, if_cond_stat),
    (elif_cond_stats_t, elifs),
    (stats_t*, else_stats)
);

SEALED_FREE_DEFINE(stat);