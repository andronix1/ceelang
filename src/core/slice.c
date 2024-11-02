#include "slice.h"

slice_t slice_mem(size_t element_size, size_t len, void *ptr) {
    slice_t result = {
        .element_size = element_size,
        .len = len,
        .ptr = ptr
    };
    return result;
}

slice_t subslice_range(slice_t *slice, size_t after, size_t before) {
    assert(after <= before && before <= slice->len);
    slice_t result = {
        .element_size = slice->element_size,
        .len = before - after,
        .ptr = slice_raw_ptr_to(slice, after)
    };
    return result;
}

slice_t subslice_after(slice_t *slice, size_t after) {
    return subslice_range(slice, after, slice->len);
}

slice_t subslice_before(slice_t *slice, size_t before) {
    return subslice_range(slice, 0, before);
}

size_t slice_len(slice_t *slice) {
    return slice->len;
}

void *slice_raw_ptr_to(slice_t *slice, size_t idx) {
    return slice->ptr + slice->element_size * idx;
}