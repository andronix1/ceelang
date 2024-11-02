#pragma once

#include <stddef.h>
#include <assert.h>

typedef struct {
    size_t element_size;
    size_t len;
    void *ptr;
} slice_t;

slice_t slice_mem(size_t element_size, size_t len, void *ptr);
slice_t subslice_range(slice_t *slice, size_t after, size_t before);
slice_t subslice_after(slice_t *slice, size_t after);
slice_t subslice_before(slice_t *slice, size_t before);
size_t slice_len(slice_t *slice);
void *slice_raw_ptr_to(slice_t *slice, size_t idx);

#define SLICE_OF(type, len, ptr) slice_mem(sizeof(type), len, ptr)

#define SLICE_FOREACH(slice, type, name, body) for (size_t i = 0; i < (slice)->len; i++) { type *name = (type*)slice_raw_ptr_to(slice, i); body }

#define SLICE_TYPED_ALIAS_DEFINE(name, type) typedef slice_t name##_t; \
    type *name##_at(name##_t *slice, size_t idx);

#define SLICE_TYPED_ALIAS_IMPL(name, type) type *name##_at(name##_t *slice, size_t idx) { return slice_raw_ptr_to(slice, idx); }