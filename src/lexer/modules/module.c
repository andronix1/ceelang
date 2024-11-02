#include "module.h"

void module_free(module_t *module) {
    str_free(&module->name);
    arr_free(&module->scopes);
}

ARR_TYPED_ALIAS_IMPL(modules, module_t, module_free);