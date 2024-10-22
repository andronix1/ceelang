#include "str.h"

str_t str_read_line(FILE* stream) {
	str_t result = str_with_cap(1);
	char c;
	while ((c = fgetc(stream)) != '\n' && !feof(stream)) {
		str_push(&result, c);
	}
	return result;
}

str_slice_t str_slice_ltrim(str_slice_t *slice, bool(*f)(char)) {
	size_t start = 0;
	while (start < slice->len && f(str_slice_at(slice, start))) start++;
	return subslice_after(slice, start);
}

str_slice_t str_slice_from_cstr(char *cstr) {
	str_slice_t result = {
		.size = 1,
		.len = strlen(cstr),
		.ptr = cstr
	};
	return result;
}

bool str_slice_starts_with(str_slice_t *slice, str_slice_t *pattern) {
	if (slice->len < pattern->len) {
		return false;
	}
	return !memcmp(slice->ptr, pattern->ptr, pattern->len);
}

void str_slice_dump(str_slice_t *str, FILE *stream) {
	fwrite(str->ptr, 1, str->len, stream);
}

uint64_t str_slice_to_uint64(str_slice_t *slice) {
	uint64_t result = 0;
	for (size_t i = 0; i < slice->len; i++) {
		result *= 10;
		result += str_slice_at(slice, i) - '0';
	}
	return result;
}
