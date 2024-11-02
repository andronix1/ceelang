#include "arr.h"

arr_t arr_new(size_t element_size, size_t cap, void (*free)(void*)) {
    arr_t result = {
        .slice = slice_mem(element_size, 0, malloc(element_size * cap)),
        .cap = cap,
        .free = free
    };
    return result;
}

arr_t arr_realloc(arr_t *arr, size_t cap) {
    arr->slice.ptr = realloc(arr->slice.ptr, arr->slice.element_size * cap);
    arr->cap = cap;
}

void arr_push(arr_t *arr, void *value) {
    if (arr->slice.len == arr->cap) {
        arr_realloc(arr, arr->cap * 2);
    }
    memcpy(slice_raw_ptr_to(&arr->slice, arr->slice.len), value, arr->slice.element_size);
    arr->slice.len++;
}

void arr_remove_at(arr_t *arr, size_t idx) {
    assert(idx < arr->slice.len);
    arr->slice.len--;
    if (arr->slice.len != idx) {
        memcpy(slice_raw_ptr_to(&arr->slice, idx), slice_raw_ptr_to(&arr->slice, idx + 1), arr->slice.element_size * (arr->slice.len - idx));
    }
    if (arr->cap > 1 && arr->slice.len * 2 < arr->cap) {
        arr_realloc(arr, arr->cap / 2);
    }
}

arr_t arr_copy_from_slice(slice_t* slice, void (*free)(void*)) {
    if (slice->len == 0) {
        return arr_new(slice->element_size, DEFAULT_CAPACITY, free);
    }
    arr_t result = {
        .slice = slice_mem(slice->element_size, slice->len, malloc(slice->element_size * slice->len)),
        .cap = slice->len,
        .free = free
    };
    memcpy(result.slice.ptr, slice->ptr, slice->element_size * slice->len);
    return result;
}

slice_t *arr_slice(arr_t *arr) {
    return &arr->slice;
}

void arr_free(arr_t *arr) {
    if (arr->free) {
        SLICE_FOREACH(&arr->slice, void, element, {
            arr->free(element);
        });
    }
    free(arr->slice.ptr);
}