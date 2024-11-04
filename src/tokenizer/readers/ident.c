#include "core/str.h"
#include "chars.h"
#include "reader.h"

token_read_result_t ident_reader(str_slice_t *slice, message_base_t base, result_t *result) {
    if (slice->len == 0 || !is_start_ident(*str_slice_at(slice, 0))) {
        return NOT_THIS();
    }
    size_t len = 0;
    while (len < slice->len && is_ident(*str_slice_at(slice, len))) len++;
    str_slice_t ident_slice = subslice_before(slice, len);
    return (token_read_result_t)token_read_result_ok_new(
        read_result_base_new_simple(),
        (token_t)token_ident_new(token_base_new_simple(location_new(0, 0)), str_copy_from_slice(&ident_slice)),
        len
    );
}
