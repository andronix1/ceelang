#pragma once

#include "core/arr.h"

typedef enum {
	DEF_CONTENT_FUNC,
	DEF_CONTENT_CONST,
} def_content_type_t;

typedef struct {
	def_content_type_t type;
} def_content_base_t;

typedef def_content_base_t* def_content_t;

