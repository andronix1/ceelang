#pragma once

#include "defs/def.h"
#include "core/str.h"
#include "core/arr.h"

typedef struct {
	str_t name;
	defs_t defs;
} scope_t;

typedef arr_t scopes_t;