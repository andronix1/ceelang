#include "parse.h"

reader_t raw_expr_readers[] = {
    raw_expr_operator_reader,
    raw_expr_fun_reader,
    raw_expr_ident_reader,
    raw_expr_scope_reader,
    raw_expr_int_reader,
    raw_expr_str_reader,
};
#define RAW_EXPR_READERS_COUNT (sizeof(raw_expr_readers) / sizeof(raw_expr_readers[0]))

void raw_expr_parse(tokens_slice_t slice, message_base_t base, result_t *result, raw_expr_t *raw_expr) {
	READING_SETUP;
	READING_LOOP {
		READING_ITER_SETUP;
		READING_SETUP_FINISH;
		base.location = (*tokens_slice_at(&slice, i))->location;
		READERS_LOOP(RAW_EXPR_READERS_COUNT) {
			READERS_TRY_READ(raw_expr_readers) {
                raw_expr_read_result_ok_t *ok = read_result_as_ok(read_result);
				raw_expr_element_t element = raw_expr_read_result_ok_extract(ok);
                arr_push(raw_expr, &element);
			}
			READERS_READ_END;
		}
		READING_ITER_FINISH(ERROR_INVALID_TOKEN);
        READER_CHANGE_POS_ON_NOT_FOUND {
			i = slice.len;
        }
	}
}

bool try_raw_expr_to_expr(raw_expr_t *raw_expr, message_base_t base, result_t *result, expr_t *expr);

bool try_raw_expr_element_to_expr(raw_expr_element_t element, message_base_t base, result_t *result, expr_t *expr) {
    SEALED_ASSERT_ALL_USED(raw_expr_element, 3);
    switch (element->kind) {
        case RAW_EXPR_OPERATOR:
            return false;
        case RAW_EXPR_READY:
            *expr = raw_expr_element_as_ready(element)->expr;
            return true;
        case RAW_EXPR_SCOPE: {
            raw_expr_element_scope_t *scope = raw_expr_element_as_scope(element);
            return try_raw_expr_to_expr(&scope->raw_expr, base, result, expr);
        }
    }
}

void raw_expr_collect_binops(raw_expr_t *raw_expr, message_base_t base, result_t *result, bool(*filter)(binop_type_t)) {
    for (size_t i = 1; i + 1 < raw_expr->slice.len; i++) {
        raw_expr_element_t element = *raw_expr_slice_at(&raw_expr->slice, i);
        if (element->kind != RAW_EXPR_OPERATOR) {
            continue;
        }
        raw_expr_element_operator_t *operator = raw_expr_element_as_operator(element);
        if (!filter(operator->type)) {
            continue;
        }
        expr_t prev_expr, next_expr;
        if (!try_raw_expr_element_to_expr(*raw_expr_slice_at(&raw_expr->slice, i - 1), base, result, &prev_expr) || 
            !try_raw_expr_element_to_expr(*raw_expr_slice_at(&raw_expr->slice, i + 1), base, result, &next_expr)) {
            continue;
        }
        raw_expr_element_free(*raw_expr_slice_at(&raw_expr->slice, i - 1));
        *raw_expr_slice_at(&raw_expr->slice, i - 1) = (raw_expr_element_t)raw_expr_element_ready_new(
            raw_expr_element_base_new_simple(),
            (expr_t)expr_binop_new(
                expr_base_new_simple(),
                operator->type,
                prev_expr,
                next_expr
            )
        );
        arr_remove_at(raw_expr, i);
        arr_remove_at(raw_expr, i);
        i--;
    }
}

bool binop_1(binop_type_t type) { return type == BINOP_DIVIDE || type == BINOP_MULTIPLY; }
bool binop_2(binop_type_t type) { return type == BINOP_MINUS || type == BINOP_PLUS; }
bool binop_3(binop_type_t type) { return type == BINOP_EQUALS || type == BINOP_NOT_EQUALS; }
bool binop_4(binop_type_t type) { return type == BINOP_LE || type == BINOP_GE || type == BINOP_LESS || type == BINOP_GREATER; }

bool try_raw_expr_to_expr(raw_expr_t *raw_expr, message_base_t base, result_t *result, expr_t *expr) {
    raw_expr_collect_binops(raw_expr, base, result, binop_1);
    raw_expr_collect_binops(raw_expr, base, result, binop_2);
    raw_expr_collect_binops(raw_expr, base, result, binop_3);
    raw_expr_collect_binops(raw_expr, base, result, binop_4);

    if (raw_expr->slice.len != 1) {
        PUSH_ERROR(ERROR_INVALID_EXPR_MISSING_BINOP);
        return false;
    }
    raw_expr_element_t element = *raw_expr_slice_at(&raw_expr->slice, 0);
    if (element->kind != RAW_EXPR_READY) {
        PUSH_ERROR(ERROR_INVALID_EXPR_BINOP);
        return false;
    }
    *expr = raw_expr_element_as_ready(element)->expr;
    return true;
}

void expr_parse(tokens_slice_t slice, message_base_t base, result_t *result, expr_t *expr) {
    raw_expr_t raw_expr = raw_expr_new_with_cap(1);
    raw_expr_parse(slice, base, result, &raw_expr);
    try_raw_expr_to_expr(&raw_expr, base, result, expr);
    arr_free(&raw_expr);
}