#pragma once

#include "core/arr.h"
#include "core/sealed.h"
#include "func/args.h"
#include "func/stats/stat.h"

SEALED_KIND(def_content,
	DEF_CONTENT_FUNC,
	DEF_CONTENT_DEFINE
);

SEALED_BASE(def_content);

SEALED_CHILD_DEFINE_FREE(def_content, DEF_CONTENT_DEFINE, define, 
	(definition_t*, definition)
);

SEALED_CHILD_DEFINE_FREE(def_content, DEF_CONTENT_FUNC, func,
	(str_t*, return_type),
	(func_args_t, args),
	(stats_t, stats)
);

SEALED_FREE_DEFINE(def_content);
