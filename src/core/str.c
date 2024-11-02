#include "str.h"

ARR_TYPED_ALIAS_IMPL(str, char, NULL)

str_t str_read_line(FILE *stream) {
	str_t result = str_new();
	char c;
	while ((c = fgetc(stream)) != '\n' && !feof(stream)) {
		str_push(&result, c);
	}
	return result;
}

str_t str_clone(str_t *target) {
	return str_copy_from_slice(&target->slice);
}

uint64_t str_slice_to_uint64(str_slice_t *slice) {
	uint64_t result = 0;
	SLICE_FOREACH(slice, char, c, {
		result *= 10;
		result += *c - '0';
	});
	return result;
}

str_slice_t str_slice_from_cstr(const char *cstr) {
	return slice_mem(sizeof(char), strlen(cstr), (void*)cstr);
}

str_slice_t str_slice_trim_left(str_slice_t *slice, bool(*filter)(char)) {
	size_t i = 0;
	while (i < slice->len && filter(*str_slice_at(slice, i))) i++;
	return subslice_after(slice, i);
}

str_slice_t str_slice_after_char(str_slice_t *slice, bool(*filter)(char)) {
	size_t i = 0;
	while (i < slice->len && !filter(*str_slice_at(slice, i))) i++;
	return subslice_after(slice, i);
}

str_slice_t str_slice_before_char(str_slice_t *slice, bool(*filter)(char)) {
	size_t i = 0;
	while (i < slice->len && !filter(*str_slice_at(slice, i))) i++;
	return subslice_before(slice, i);
}

bool str_slice_starts_with(str_slice_t *slice, str_slice_t *other) {
	if (other->len > slice->len) {
		return false;
	}
	for (size_t i = 0; i < other->len; i++) {
		if (*str_slice_at(other, i) != *str_slice_at(slice, i)) {
			return false;
		}
	}
	return true;
}

str_slice_t str_slice_dump(str_slice_t *slice, FILE* stream) {
	fwrite(slice->ptr, slice->element_size, slice->len, stream);
}