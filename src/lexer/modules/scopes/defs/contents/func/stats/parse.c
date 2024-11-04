#include "parse.h"

reader_t stat_readers[] = {
	stat_define_parse,
	stat_funcall_parse,
	stat_return_parse
};
#define STAT_READERS_COUNT (sizeof(stat_readers) / sizeof(stat_readers[0]))

void stats_parse(tokens_slice_t slice, message_base_t base, result_t *result, stats_t *stats) {
	READING_SETUP;
	READING_LOOP {
		READING_ITER_SETUP;
		READING_SETUP_FINISH;
		base.location = (*tokens_slice_at(&slice, i))->location;
		READERS_LOOP(STAT_READERS_COUNT) {
			READERS_TRY_READ(stat_readers) {
                stat_read_result_ok_t *ok = read_result_as_ok(read_result);
				stat_t stat = stat_read_result_ok_extract(ok);
                arr_push(stats, &stat);
			}
			READERS_READ_END;
		}
		READING_ITER_FINISH(ERROR_INVALID_TOKEN);
        READER_CHANGE_POS_ON_NOT_FOUND {
            while (i < slice.len && (*tokens_slice_at(&slice, i))->kind != TOKEN_SEMICOLON) i++;
			i++;
        }
	}
}