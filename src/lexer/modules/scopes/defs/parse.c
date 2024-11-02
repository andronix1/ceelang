#include "parse.h"

#define EXPECT_TOKEN(token, type) \
    do { \
        if ((token)->kind != type) { \
            result_push(result, (message_t)message_error_new(base, ERROR_INVALID_TOKEN)); \
            return (def_read_result_t)def_read_result_invalid_new(def_read_result_base_new_simple(), tokens->len); \
        } \
    } while (0)

#define EXPECT_TOKENS_COUNT(count) \
    do { \
        if (tokens->len < (count)) { \
            result_push(result, (message_t)message_error_new(base, ERROR_MISSING_TOKEN)); \
            return (def_read_result_t)def_read_result_invalid_new(def_read_result_base_new_simple(), tokens->len); \
        } \
    } while (0)

token_t token_next(size_t *len, tokens_slice_t *tokens, message_base_t *base) {
    token_t token = *tokens_slice_at(tokens, (*len)++);
    base->location = token->location;
    return token;
}

#define NEXT_TOKEN() token_next(&len, tokens, &base)

def_read_result_t fun_parse(tokens_slice_t *tokens, message_base_t base, result_t *result) {
    size_t len = 0;
    if (tokens->len < 1) {
        return def_read_result_base_new(DEF_READ_NOT_THIS);
    }
    token_t def_token = NEXT_TOKEN();
    if (def_token->kind != TOKEN_FUN) {
        return def_read_result_base_new(DEF_READ_NOT_THIS);
    }

    // fun <IDENT> (
    EXPECT_TOKENS_COUNT(4);
    token_t fun_name_token = NEXT_TOKEN();
    EXPECT_TOKEN(fun_name_token, TOKEN_IDENT);
    EXPECT_TOKEN(NEXT_TOKEN(), TOKEN_OPENING_CIRCLE_BRACE);
    
    // <ARGS> )
    func_args_t args = func_args_new_with_cap(1);
    token_t token;
    while ((token = NEXT_TOKEN())->kind != TOKEN_CLOSING_CIRCLE_BRACE) {
        EXPECT_TOKENS_COUNT(len + 3);
        token_t name_token = token;
        EXPECT_TOKEN(name_token, TOKEN_IDENT);
        EXPECT_TOKEN(NEXT_TOKEN(), TOKEN_COLON);
        token_t type_token = NEXT_TOKEN();
        EXPECT_TOKEN(type_token, TOKEN_IDENT);
        func_arg_t arg = {
            .name = str_clone(&token_as_ident(name_token)->ident),
            .type = str_clone(&token_as_ident(type_token)->ident)
        };
        arr_push(&args, &arg);
    }

    // : <TYPE> {
    str_t *return_type = NULL;
    EXPECT_TOKENS_COUNT(len + 1);
    token_t body_opening_token = NEXT_TOKEN();
    if (body_opening_token->kind == TOKEN_COLON) {
        EXPECT_TOKENS_COUNT(len + 2);
        token_t return_type_token = NEXT_TOKEN();
        EXPECT_TOKEN(return_type_token, TOKEN_IDENT);
        return_type = malloc(sizeof(str_t));
        *return_type = str_clone(&token_as_ident(return_type_token)->ident);
        body_opening_token = NEXT_TOKEN();
    }
    EXPECT_TOKEN(body_opening_token, TOKEN_OPENING_FIGURE_BRACE);
    EXPECT_TOKEN(NEXT_TOKEN(), TOKEN_CLOSING_FIGURE_BRACE);

    str_t fun_name = token_as_ident(fun_name_token)->ident;

    def_t def = {
        .content = (def_content_t)def_content_func_new(def_content_base_new_simple(), return_type, args, stats_new_with_cap(1)),
        .name = str_clone(&fun_name)
    };

    return (def_read_result_t)def_read_result_ok_new(def_read_result_base_new_simple(), def, len);
}

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
            SEALED_ASSERT_ALL_USED(def_read_result, 3);
            if (read_result->kind == DEF_READ_ERR_INVALID) {
                def_read_result_invalid_t *invalid = def_read_result_as_invalid(read_result);
                token_number += invalid->len;
				found = true;
			} else if (read_result->kind == DEF_READ_OK) {
                def_read_result_ok_t *ok = def_read_result_as_ok(read_result);
                arr_push(defs, &ok->def);
                token_number += ok->len;
				found = true;
			}
            def_read_result_free(read_result);
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