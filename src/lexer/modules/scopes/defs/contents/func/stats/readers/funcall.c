#include "funcall.h"

stat_read_result_t stat_funcall_parse(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    READER_SETUP;
    funcall_read_result_t funcall_parsed = funcall_reader(tokens, base, result);
    if (funcall_parsed->kind != READ_OK) {
        return funcall_parsed;
    }
    funcall_read_result_ok_t *ok = read_result_as_ok(funcall_parsed);
    funcall_t *funcall = funcall_read_result_ok_extract(ok);
    len = ok->len;
    read_result_free(funcall_parsed);
    EXPECT_TOKEN(NEXT_TOKEN(), TOKEN_SEMICOLON);
    return (stat_read_result_t)stat_read_result_ok_new(read_result_base_new_simple(), (stat_t)stat_funcall_new(stat_base_new_simple(), funcall), len);
}