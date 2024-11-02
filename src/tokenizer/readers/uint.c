#include "core/str.h"
#include "chars.h"
#include "reader.h"

token_read_result_t uint_reader(str_slice_t *slice) {
    size_t len = 0;
    while (len < slice->len && is_digit(*str_slice_at(slice, len))) len++;
    if (len == 0) {
        return (token_read_result_t)token_read_result_base_new(TOKEN_READ_NOT_THIS);
    }
    str_slice_t uint_slice = subslice_before(slice, len);
    return (token_read_result_t)token_read_result_ok_new(
        token_read_result_base_new_simple(),
        (token_t)token_uint_new(token_base_new_simple(location_new(0, 0)), str_slice_to_uint64(&uint_slice)),
        len,
        false,
        warning_variants_count
    );
}
