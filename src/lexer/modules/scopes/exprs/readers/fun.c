#include "fun.h"

raw_expr_read_result_t raw_expr_fun_reader(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    READER_SETUP;
    funcall_read_result_t funcall_parsed = funcall_reader(tokens, base, result);
    if (funcall_parsed->kind != READ_OK) {
        return funcall_parsed;
    }
    funcall_read_result_ok_t *ok = read_result_as_ok(funcall_parsed);
    funcall_t *funcall = funcall_read_result_ok_extract(ok);
    len = ok->len;
    read_result_free(funcall_parsed);
    return (raw_expr_read_result_t)raw_expr_read_result_ok_new(
        read_result_base_new_simple(),
        (raw_expr_element_t)raw_expr_element_ready_new(
            raw_expr_element_base_new_simple(),
            (expr_t)expr_funcall_new(
                expr_base_new_simple(),
                funcall
            )
        ),
        len
    );
}