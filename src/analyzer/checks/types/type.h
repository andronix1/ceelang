#pragma once

#include "core/sealed.h"
#include "core/str.h"

SEALED_KIND(type,
	TYPE_SIMPLE
);

SEALED_BASE(type,
	(str_slice_t, name)
);

ARR_TYPED_ALIAS_DEFINE(types, type_t);

typedef struct {
	types_t builtin;
} types_info_t;

types_info_t types_info_new();

type_t type_resolve(types_info_t *types, str_slice_t *name);
