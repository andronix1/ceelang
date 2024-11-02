#include "def.h"

void def_free(def_t *def) {
    def_content_free(def->content);
    str_free(&def->name);
}

ARR_TYPED_ALIAS_IMPL(defs, def_t, def_free);