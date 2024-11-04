#include "definition.h"

void definition_free(definition_t *definition) {
    if (definition->expr) {
        expr_free(definition->expr);
    }
    str_free(&definition->type);
}