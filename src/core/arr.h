#pragma once

#include <malloc.h>
#include <assert.h>
#include <string.h>

typedef struct {
	size_t len;
	size_t size;
	void *ptr;
} slice_t;

typedef struct {
	slice_t slice;
	size_t cap;
} arr_t;

void arr_reserve(arr_t *arr, size_t cap);
arr_t arr_with_cap_sized(size_t size, size_t cap);
arr_t arr_copy_from_slice(slice_t *slice);
slice_t subslice_before(slice_t *slice, size_t before);
slice_t subslice_after(slice_t *slice, size_t after);
void *arr_get_ptr(arr_t *arr, size_t idx);
void *slice_get_ptr(slice_t *slice, size_t idx);
void arr_push_unsafe(arr_t *arr, void *value);
void arr_free(arr_t *arr);

#define arr_len(arr) (arr)->slice.len
#define arr_at(type, arr, i) (*(type*)arr_get_ptr(arr, i))
#define slice_at(type, arr, i) (*(type*)slice_get_ptr(arr, i))
#define arr_with_cap(type, cap) arr_with_cap_sized(sizeof(type), cap)
#define arr_push(type, arr, value) do { type _value = value; arr_push_unsafe(arr, &_value); } while(0)
#define arr_foreach(type, arr, value, body) do { \
		for (size_t value##_idx = 0; value##_idx < arr_len(arr); value##_idx++) {	\
			type *value = (type*)arr_get_ptr(arr, value##_idx);			\
			body 								\
		} 									\
	} while(0)

