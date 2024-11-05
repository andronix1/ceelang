#include "types.h"

ARR_TYPED_ALIAS_IMPL(types_scope, type_t, NULL);

bool types_scope_has_type(types_scope_slice_t *scope, str_slice_t *type) {
	SLICE_FOREACH(scope, type_t, t, {
		if (str_slice_equals(type, &(*t)->name)) {
			return true;
		}
	});
	return false;
}
