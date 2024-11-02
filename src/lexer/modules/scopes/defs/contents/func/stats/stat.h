#pragma once

#include "lexer/modules/scopes/exprs/funcall/funcall.h"
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

SEALED_CHILD_DEFINE(stat, STAT_DEFINE, define,
    (stat_define_type_t, define_type),
    (str_t, name),
    (str_t, type),
    (expr_t, expr)
);

SEALED_CHILD_DEFINE(stat, STAT_FUNCALL, funcall,
    (funcall_t, funcall)
);

SEALED_CHILD_DEFINE(stat, STAT_RETURN, return,
    (expr_t, expr)
);

SEALED_CHILD_DEFINE(stat, STAT_IF, if,
    (expr_t, cond),
    (stats_t, if_stats),
    (stats_t*, else_stats)
);

SEALED_FREE_DEFINE(stat);