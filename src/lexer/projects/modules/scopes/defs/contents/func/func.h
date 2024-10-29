#pragma once

#include "core/str.h"
#include "args.h"
#include "stats/stat.h"
#include "../content.h"

typedef struct {
	def_content_type_t type;
	str_t *return_type; // OPTIONAL
	func_args_t args;
	stats_t stats;
} def_content_func_t;
