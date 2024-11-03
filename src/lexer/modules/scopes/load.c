#include "load.h"

bool is_separator(char c) { return c == '/'; }
bool is_dot(char c) { return c == '.'; }

void scope_load(char *path, message_base_t base, result_t *result, scope_t *scope) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("ERROR: failed to open file: %s", strerror(errno));
    }

    str_slice_t path_slice = str_slice_from_cstr(path);

    slice_t filename = str_slice_after_char(&path_slice, is_separator);
    filename = subslice_after(&filename, 1);
    scope->name = str_copy_from_slice(&filename);

    tokens_t tokens = tokens_new_with_cap(1);
    tokenize(file, base, result, &tokens);
	fclose(file);
    
    scope->defs = defs_new_with_cap(1);
    defs_parse(tokens.slice, base, result, &scope->defs);

    arr_free(&tokens);
}