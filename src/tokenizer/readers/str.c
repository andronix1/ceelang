#include "core/str.h"
#include "chars.h"
#include "reader.h"

token_read_result_t str_reader(str_slice_t *slice, message_base_t base, result_t *result) {
    if (slice->len == 0 || *str_slice_at(slice, 0) != '\"') {
        return NOT_THIS();
    }
    size_t len = 1;
    // TODO: escaping
    while (len < slice->len && *str_slice_at(slice, len) != '\"') len++;
    if (len == slice->len) {
        PUSH_ERROR(error_base_new(ERROR_STRING_NOT_CLOSED));
        return INVALID(slice->len);
    }
    str_slice_t str_slice = subslice_before(slice, len);
    str_slice = subslice_after(&str_slice, 1);
    return (token_read_result_t)token_read_result_ok_new(
        read_result_base_new_simple(),
        (token_t)token_str_new(token_base_new_simple(location_new(0, 0)), str_copy_from_slice(&str_slice)),
        len + 1
    );
}
