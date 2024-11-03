#include "parse.h"

reader_t def_readers[] = {
    fun_parse
};
#define DEF_READERS_COUNT (sizeof(def_readers) / sizeof(def_readers[0]))

void defs_parse(tokens_slice_t slice, message_base_t base, result_t *result, defs_t *defs) {
	READING_SETUP;
	READING_LOOP {
		READING_ITER_SETUP;
		READING_SETUP_FINISH;
		base.location = (*tokens_slice_at(&slice, i))->location;
		READERS_LOOP(DEF_READERS_COUNT) {
			READERS_TRY_READ(def_readers) {
                def_read_result_ok_t *ok = read_result_as_ok(read_result);
                def_t *def = def_read_result_ok_extract(ok);
                arr_push(defs, def);
                i += ok->len;
			}
			READERS_READ_END;
		}
		READING_ITER_FINISH(ERROR_INVALID_TOKEN);
        READER_CHANGE_POS_ON_NOT_FOUND {
            i++;
        }
	}
}