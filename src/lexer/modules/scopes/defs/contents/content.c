#include "content.h"

SEALED_CHILD_IMPL_FREE(def_content, func, {
	if (self->return_type) {
		str_free(self->return_type);
		free(self->return_type);
	}
	arr_free(&self->stats);
	arr_free(&self->args);
});

SEALED_CHILD_IMPL_FREE(def_content, define, {
	definition_free(self->definition);
	free(self->definition);
});

SEALED_FREE_IMPL(def_content, {
	SEALED_SHOULD_BE_FREED(def_content, DEF_CONTENT_FUNC, func)
	SEALED_SHOULD_BE_FREED(def_content, DEF_CONTENT_DEFINE, define)
}, 2);