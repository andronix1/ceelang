#include "define.h"

stat_read_result_t stat_define_parse(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    READER_SETUP;
    str_t name;
    definition_read_result_t definition_parsed = definition_parse(tokens, base, result, &name);
    if (definition_parsed->kind != READ_OK) {
        return definition_parsed;
    }
    definition_read_result_ok_t *ok = read_result_as_ok(definition_parsed);
    definition_t *definition = definition_read_result_ok_extract(ok);
    len = ok->len;
    read_result_free(definition_parsed);
    return (stat_read_result_t)stat_read_result_ok_new(read_result_base_new_simple(),
        (stat_t)stat_define_new(stat_base_new_simple(), definition, name),
        len
    );
}