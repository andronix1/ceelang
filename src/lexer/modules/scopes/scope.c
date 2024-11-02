#include "scope.h"

void scope_free(scope_t *scope) {
    str_free(&scope->name);
    arr_free(&scope->defs);
}

ARR_TYPED_ALIAS_IMPL(scopes, scope_t, scope_free);