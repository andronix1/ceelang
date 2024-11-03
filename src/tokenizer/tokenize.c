#include "tokenize.h"

DEFINE_KEYWORD(const, TOKEN_CONST, "const")
DEFINE_KEYWORD(var, TOKEN_VAR, "var")
DEFINE_KEYWORD(fun, TOKEN_FUN, "fun")
DEFINE_KEYWORD(if, TOKEN_IF, "if")
DEFINE_KEYWORD(elif, TOKEN_ELIF, "elif")
DEFINE_KEYWORD(else, TOKEN_ELSE, "else")
DEFINE_KEYWORD(return, TOKEN_RETURN, "return")

DEFINE_SYMBOL(set, TOKEN_SET, "=")
DEFINE_SYMBOL(not_equals, TOKEN_NOT_EQUALS, "!=")
DEFINE_SYMBOL(equals, TOKEN_EQUALS, "==")
DEFINE_SYMBOL(greater, TOKEN_GREATER, ">")
DEFINE_SYMBOL(less, TOKEN_LESS, "<")
DEFINE_SYMBOL(ge, TOKEN_GE, ">=")
DEFINE_SYMBOL(le, TOKEN_LE, "<=")

DEFINE_SYMBOL(mod, TOKEN_MOD, "%")
DEFINE_SYMBOL(plus, TOKEN_PLUS, "+")
DEFINE_SYMBOL(minus, TOKEN_MINUS, "-")
DEFINE_SYMBOL(multiply, TOKEN_MULTIPLY, "*")
DEFINE_SYMBOL(divide, TOKEN_DIVIDE, "/")

DEFINE_SYMBOL(colon, TOKEN_COLON, ":")
DEFINE_SYMBOL(semicolon, TOKEN_SEMICOLON, ";")
DEFINE_SYMBOL(comma, TOKEN_COMMA, ",")
DEFINE_SYMBOL(opening_circle_brace, TOKEN_OPENING_CIRCLE_BRACE, "(")
DEFINE_SYMBOL(opening_figure_brace, TOKEN_OPENING_FIGURE_BRACE, "{")
DEFINE_SYMBOL(closing_circle_brace, TOKEN_CLOSING_CIRCLE_BRACE, ")")
DEFINE_SYMBOL(closing_figure_brace, TOKEN_CLOSING_FIGURE_BRACE, "}")

reader_t token_readers[] = {
	// keywords
	keyword_const_reader, keyword_var_reader,
	keyword_fun_reader,
	keyword_if_reader, keyword_elif_reader, keyword_else_reader,
	keyword_return_reader,

	// operators
	symbol_not_equals_reader, symbol_equals_reader,
	symbol_ge_reader, symbol_greater_reader,
	symbol_le_reader, symbol_less_reader,
	symbol_mod_reader,
	symbol_divide_reader, symbol_multiply_reader,
	symbol_minus_reader, symbol_plus_reader,
	symbol_set_reader,

	// symbols
	symbol_colon_reader,
	symbol_semicolon_reader,
	symbol_comma_reader,

	// braces
	symbol_opening_circle_brace_reader, symbol_closing_circle_brace_reader,
	symbol_opening_figure_brace_reader, symbol_closing_figure_brace_reader,

	// parametrized
	ident_reader,
	uint_reader,
};
#define TOKEN_READERS_COUNT (sizeof(token_readers) / sizeof(token_readers[0]))

void tokenize_line(str_slice_t slice, message_base_t base, result_t *result, tokens_t *tokens) {
	READING_SETUP;
	READING_LOOP {
		READING_ITER_SETUP;
		cur_slice = str_slice_trim_left(&cur_slice, is_whitespace);
		if (cur_slice.len == 0 || *str_slice_at(&cur_slice, 0) == '#') {
			break;
		}
		READING_SETUP_FINISH;
		base.location.character = i;
		READERS_LOOP(TOKEN_READERS_COUNT) {
			READERS_TRY_READ(token_readers) {
				token_read_result_ok_t *ok = read_result_as_ok(read_result);
				token_t token = token_read_result_ok_extract(ok);
				token->location = base.location;
				arr_push(tokens, &token);
			}
			READERS_READ_END;
		}
		READING_ITER_FINISH(ERROR_UNKNOWN_TOKEN);
        READER_CHANGE_POS_ON_NOT_FOUND {
            i++;
        }
	}
}

void tokenize(FILE* stream, message_base_t base, result_t *result, tokens_t *tokens) {
	size_t line_number = 0;
	while (!feof(stream)) {
		str_t line = str_read_line(stream);
		base.location = location_new(line_number, 0);
        tokenize_line(line.slice, base, result, tokens);
        str_free(&line);
		line_number++;
	}
}