#pragma once

#include <stddef.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include "slice.h"

#define DEFAULT_CAPACITY 1

typedef struct {
    slice_t slice;
    size_t cap;
    void (*free)(void*);
} arr_t;

arr_t arr_new(size_t len, size_t capacity, void (*free)(void*));
arr_t arr_copy_from_slice(slice_t* slice, void (*free)(void*));
void arr_pop(arr_t *arr);
void arr_push(arr_t *arr, void *value);
void arr_remove_at(arr_t *arr, size_t idx);
slice_t *arr_slice(arr_t *arr);
void arr_free(arr_t *arr);

#define ARR_TYPED_ALIAS_DEFINE(name, type) \
    SLICE_TYPED_ALIAS_DEFINE(name##_slice, type) \
    typedef arr_t name##_t; \
    name##_t name##_new_with_cap(size_t capacity); \
    name##_t name##_copy_from_slice(name##_slice_t* slice);

#define ARR_TYPED_ALIAS_IMPL(name, type, free) \
    name##_t name##_new_with_cap(size_t capacity) { return arr_new(sizeof(type), capacity, (void(*)(void*))free); } \
    name##_t name##_copy_from_slice(name##_slice_t* slice) { return arr_copy_from_slice(slice, (void(*)(void*))free); } \
    SLICE_TYPED_ALIAS_IMPL(name##_slice, type)
