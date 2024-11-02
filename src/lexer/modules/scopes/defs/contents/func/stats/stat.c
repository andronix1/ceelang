#include "stat.h"

ARR_TYPED_ALIAS_IMPL(stats, stat_t, stat_free);

SEALED_CHILD_IMPL(stat, define);
SEALED_CHILD_IMPL(stat, funcall);
SEALED_CHILD_IMPL(stat, return);
SEALED_CHILD_IMPL(stat, if);

SEALED_FREE_IMPL(stat, {}, 4);