#include "parse.h"

def_reader_t def_readers[] = {
    fun_parse
};
#define READERS_COUNT (sizeof(def_readers) / sizeof(def_readers[0]))

void defs_parse(tokens_slice_t tokens, message_base_t base, result_t *result, defs_t *defs) {
	size_t token_number = 0;
    while (token_number < tokens.len) {
		bool found = false, invalid_last_time = false;
		tokens_slice_t cur_slice = subslice_after(&tokens, token_number);
        for (size_t i = 0; i < READERS_COUNT; i++) {
            def_read_result_t read_result = def_readers[i](&cur_slice, base, result);
            if (read_result->kind == READ_ERR_INVALID) {
                read_result_invalid_t *invalid = read_result_as_invalid(read_result);
                token_number += invalid->len;
				found = true;
			} else if (read_result->kind == READ_OK) {
                def_read_result_ok_t *ok = read_result_as_ok(read_result);
                def_t *def = def_read_result_ok_extract(ok);
                arr_push(defs, def);
                token_number += ok->len;
				found = true;
			}
            read_result_free(read_result);
			if (found) {
				break;
			}
        }
		if (!found) {
			if (!invalid_last_time) {
				result_push(result, (message_t)message_error_new(base, ERROR_INVALID_TOKEN));
				invalid_last_time = true;
			}
			token_number++;
		} else {
			invalid_last_time = false;
		}
    }
}