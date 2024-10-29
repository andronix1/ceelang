#include "parse.h"

stat_t funcall_stat_parse(tokens_slice_t tokens) {
    stat_funcall_t *result = malloc(sizeof(stat_funcall_t));
    result->base.type = STAT_FUNCALL;
    result->funcall = funcall_parse(tokens);
    return (stat_t)result;
}