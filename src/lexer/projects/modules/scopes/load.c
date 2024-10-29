#include "load.h"

scope_t scope_load(char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("ERROR: failed to open file: %s", strerror(errno));
        exit(1);
    }

    tokens_t tokens = tokenize(file);
    defs_t defs = defs_parse(tokens.slice);

    slice_t path_slice = str_slice_from_cstr(path);
    slice_t filename = str_slice_right_part(&path_slice, '/');
    scope_t result = {
        .name = str_copy_from_slice(&filename),
        .defs = defs
    };
    return result;
}