#pragma once

#include "core/str.h"
#include "core/arr.h"
#include "contents/content.h"

typedef struct {
	str_t name;
	def_content_t content;
} def_t;

ARR_TYPED_ALIAS_DEFINE(defs, def_t);
