#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "arr.h"

typedef arr_t str_t;
typedef slice_t str_slice_t;

str_t str_read_line(FILE* stream);
str_slice_t str_slice_ltrim(str_slice_t *slice, bool(*f)(char));
str_slice_t str_slice_from_cstr(char *cstr);
bool str_slice_starts_with(str_slice_t *slice, str_slice_t *pattern);
void str_slice_dump(str_slice_t *slice, FILE *stream);
uint64_t str_slice_to_uint64(str_slice_t *slice);

#define str_copy_from_slice arr_copy_from_slice
#define str_slice_at(str, i) ((char*)(str)->ptr)[i]
#define str_with_cap(cap) arr_with_cap(char, cap)
#define str_push(str, c) arr_push(char, str, c)
#define str_free arr_free
