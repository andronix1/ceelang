#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "arr.h"

ARR_TYPED_ALIAS_DEFINE(str, char)

str_t str_read_line(FILE *stream);
str_t str_clone(str_t *target);
uint64_t str_slice_to_uint64(str_slice_t *slice);
str_slice_t str_slice_from_cstr(const char *cstr);
bool str_slice_starts_with(str_slice_t *slice, str_slice_t *other);
str_slice_t str_slice_trim_left(str_slice_t *slice, bool(*filter)(char));
str_slice_t str_slice_after_char(str_slice_t *slice, bool(*filter)(char));
str_slice_t str_slice_before_char(str_slice_t *slice, bool(*filter)(char));
str_slice_t str_slice_dump(str_slice_t *slice, FILE* stream);

static inline void str_push(str_t *str, char c) {
    arr_push(str, &c);
}

#define str_new() str_new_with_cap(1)
#define str_free arr_free