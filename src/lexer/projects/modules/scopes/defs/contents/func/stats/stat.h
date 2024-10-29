#pragma once

#include "core/arr.h"

typedef enum {
    STAT_DEFINE,
    STAT_FUNCALL,
    STAT_RETURN,
} stat_type_t;

typedef struct {
    stat_type_t type;
} stat_base_t;

typedef stat_base_t* stat_t;

typedef arr_t stats_t;