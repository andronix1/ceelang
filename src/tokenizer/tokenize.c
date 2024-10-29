#include "tokenize.h"

bool is_keyword(str_slice_t *line, str_slice_t *ident) {
	if (!str_slice_starts_with(line, ident)) {
		return false;
	}
	return line->len == ident->len || !is_ident(str_slice_at(line, ident->len));
}

// TODO: don't do slice from cstr every call
#define define_keyword(name, token_type) token_build_result_t name##_builder(str_slice_t* slice) { \
		str_slice_t ident = str_slice_from_cstr(#name);		\
		if (!is_keyword(slice, &ident)) {			\
			return token_build_is_not_this;			\
		}							\
		token_build_result_t result;				\
		token_build_ok_init(result, token_ident_t, ident.len, { \
			token->type = token_type; 			\
			token->ident = str_copy_from_slice(&ident); 	\
		});							\
		return result; 						\
	}

// TODO: don't do slice from cstr every call
#define define_op(name, value, token_type) token_build_result_t name##_builder(str_slice_t* slice) { \
		str_slice_t ident = str_slice_from_cstr(value);		\
		if (!str_slice_starts_with(slice, &ident)) {		\
			return token_build_is_not_this;			\
		}							\
		token_build_result_t result;				\
		token_build_ok_init(result, token_base_t, ident.len, { 	\
			token->type = token_type; 			\
		});							\
		return result; 						\
	}

#define define_symbol(name, symbol, token_type) token_build_result_t name##_builder(str_slice_t* slice) { \
		if (str_slice_at(slice, 0) != symbol) { 	\
			return token_build_is_not_this;		\
		}						\
		token_build_result_t result;			\
		token_build_ok_init(result, token_base_t, 1, { 	\
			token->type = token_type; 		\
		});						\
		return result; 					\
	}

token_build_result_t ident_builder(str_slice_t* slice) {
	size_t before = 0;
	if (!is_start_ident(str_slice_at(slice, 0))) {
		return token_build_is_not_this;
	}
	while (is_ident(str_slice_at(slice, before))) before++;
	str_slice_t ident = subslice_before(slice, before);
	token_build_result_t result;
	token_build_ok_init(result, token_ident_t, before, {
		token->type = TOKEN_IDENT;
		token->ident = str_copy_from_slice(&ident);
	});
	return result;
}

token_build_result_t uint_builder(str_slice_t* slice) {
	size_t before = 0;
	while (is_digit(str_slice_at(slice, before))) before++;
	if (before == 0) {
		return token_build_is_not_this;
	}
	str_slice_t num = subslice_before(slice, before);
	token_build_result_t result;
	token_build_ok_init(result, token_uint_t, before, {
		token->type = TOKEN_UINT;
		token->value = str_slice_to_uint64(&num);
	});
	return result;
}

token_build_result_t str_builder(str_slice_t* slice) {
	size_t before = 1;
	if (str_slice_at(slice, 0) != '\"') {
		return token_build_is_not_this;
	}
	while (str_slice_at(slice, before) != '\"' && before < slice->len) {
		before++;
	}
	if (before == slice->len) {
		return token_build_is_not_this;
	}
	str_slice_t value = subslice_after(slice, 1);
	value = subslice_before(&value, before - 1);
	token_build_result_t result;
	token_build_ok_init(result, token_str_t, before + 1, {
		token->type = TOKEN_STR;
		token->value = str_copy_from_slice(&value);
	});
	return result;
}

define_keyword(const, TOKEN_CONST)
define_keyword(var, TOKEN_VAR)
define_keyword(fun, TOKEN_FUN)
define_keyword(if, TOKEN_IF)
define_keyword(else, TOKEN_ELSE)
define_keyword(return, TOKEN_RETURN)
define_op(equals, "==", TOKEN_EQUALS)
define_op(not_equals, "!=", TOKEN_NOT_EQUALS)
define_op(ge, ">=", TOKEN_GE)
define_op(le, "<=", TOKEN_LE)
define_symbol(greater, '>', TOKEN_GREATER)
define_symbol(less, '<', TOKEN_LESS)
define_symbol(set, '=', TOKEN_SET)
define_symbol(mod, '%', TOKEN_MOD)
define_symbol(divide, '/', TOKEN_DIVIDE)
define_symbol(multiply, '*', TOKEN_MULTIPLY)
define_symbol(minus, '-', TOKEN_MINUS)
define_symbol(plus, '+', TOKEN_PLUS)
define_symbol(opening_figure_brace, '{', TOKEN_OPENING_FIGURE_BRACE)
define_symbol(closing_figure_brace, '}', TOKEN_CLOSING_FIGURE_BRACE)
define_symbol(opening_circle_brace, '(', TOKEN_OPENING_CIRCLE_BRACE)
define_symbol(closing_circle_brace, ')', TOKEN_CLOSING_CIRCLE_BRACE)
define_symbol(comma, ',', TOKEN_COMMA)
define_symbol(colon, ':', TOKEN_COLON)
define_symbol(semicolon, ';', TOKEN_SEMICOLON)

tokens_t tokenize(FILE *stream) {
	tokens_t result = arr_with_cap(token_t, TOKENS_CAPACITY);

	token_builder_t token_builders[] = {
		const_builder,
		var_builder,
		return_builder,
		fun_builder,
		if_builder,
		else_builder,
		opening_figure_brace_builder,
		closing_figure_brace_builder,
		opening_circle_brace_builder,
		closing_circle_brace_builder,
		comma_builder,
		colon_builder,
		//---------
		equals_builder,
		not_equals_builder,
		set_builder,
		//----------
		ge_builder,
		greater_builder,
		//----------
		le_builder,
		less_builder,
		//----------
		mod_builder,
		divide_builder,
		multiply_builder,
		minus_builder,
		plus_builder,
		ident_builder,
		uint_builder,
		str_builder,
		semicolon_builder
	};

	#define token_builders_len (sizeof(token_builders) / sizeof(token_builders[0]))

	str_slice_t comment_start = str_slice_from_cstr("#");

	while (!feof(stream)) {
		str_t line = str_read_line(stream);
		str_slice_t slice = line.slice;
		bool token_added = false;
		while ((slice = str_slice_ltrim(&slice, is_whitespace)).len != 0) {
			if (str_slice_starts_with(&slice, &comment_start)) {
				break;
			}
			bool found = false;
			for (size_t i = 0; i < token_builders_len; i++) {
				token_build_result_t bres = token_builders[i](&slice);
				if (bres.err == TOKEN_BUILD_ERR_IS_NOT_THIS) {
					continue;
				}
				switch (bres.err) {
				case TOKEN_BUILD_OK:
					arr_push(token_t, &result, bres.token);
					slice = subslice_after(&slice, bres.end);
					token_added = true;
					break;
				default:
					printf("ERROR: unknown build error: %d\n", bres.err);
					break;
				}
				found = true;
				break;
			}
			if (!found) {
				printf("ERROR: cannot recognize token at line `");
				str_slice_dump(&line.slice, stdout);
				printf("` with len %d\n", line.slice.len);
				exit(1);
			}
		}
		str_free(&line);
	}

	return result;
}
