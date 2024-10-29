#include "arr.h"

void arr_reserve(arr_t *arr, size_t cap) {
	assert(arr->cap < cap);
	arr->slice.ptr = realloc(arr->slice.ptr, arr->slice.size * cap);
	arr->cap = cap;
}

arr_t arr_with_cap_sized(size_t size, size_t cap) {
	arr_t result = {
		.slice = {
			.len = 0,
			.ptr = malloc(sizeof(size) * cap),
			.size = size,
		},
		.cap = cap,
	};
	return result;
}

arr_t arr_copy_from_slice(slice_t *slice) {
	arr_t result = {
		.slice = {
			.len = slice->len,
			.size = slice->size,
			.ptr = malloc(slice->size * slice->len)
		},
		.cap = slice->len == 0 ? 1 : slice->len
	};
	memcpy(result.slice.ptr, slice->ptr, slice->size * slice->len);
	return result;
}

void *slice_get_ptr(slice_t *slice, size_t idx) {
	assert(slice->len >= idx);
	return slice->ptr + idx * slice->size;
}

slice_t subslice_after(slice_t *slice, size_t after) {
	assert(slice->len >= after);
	slice_t result = {
		.len = slice->len - after,
		.size = slice->size,
		.ptr = slice_get_ptr(slice, after),
	};
	return result;
}

slice_t subslice_before(slice_t *slice, size_t before) {
	assert(slice->len >= before);
	slice_t result = {
		.len = before,
		.size = slice->size,
		.ptr = slice->ptr,
	};
	return result;
}

void *arr_get_ptr(arr_t *arr, size_t idx) {
	assert(idx < arr->slice.len);
	return slice_get_ptr(&arr->slice, idx);
}

void arr_push_unsafe(arr_t *arr, void *value) {
	arr->slice.len++;
	if (arr_len(arr) >= arr->cap) {
		arr_reserve(arr, arr->cap * 2);
	}
	memcpy(arr_get_ptr(arr, arr_len(arr) - 1), value, arr->slice.size);
	//fwrite(arr->ptr, 1, arr->len, stdout);
	//fputc('\n', stdout);
}

void arr_remove(arr_t *arr, size_t idx) {
	assert(idx < arr->slice.len);
	if (idx + 1 != arr->slice.len) {
		memcpy(arr_get_ptr(arr, idx), arr_get_ptr(arr, idx + 1), (arr->slice.len - idx) * arr->slice.size);
	}
	arr->slice.len--;
	if (arr->slice.len < arr->cap / 2) {
		arr->cap /= 2;
		arr->slice.ptr = realloc(arr->slice.ptr, arr->slice.size * arr->cap);
	}
}

void arr_free(arr_t *arr) {
	free(arr->slice.ptr);
}
