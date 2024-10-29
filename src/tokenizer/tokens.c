#include "tokens.h"

const char *str_token[TOKENS_COUNT] = {
	// META
	"SEMICOLON",
	// WITHOUT ARGS
	"CONST",
	"VAR",
	"RETURN",
	"FUN",
	"IF", "ELSE",
	"OPENING_FIGURE_BRACE", "CLOSING_FIGURE_BRACE",
	"OPENING_CIRCLE_BRACE", "CLOSING_CIRCLE_BRACE",
	"COMMA",
	"COLON",
	//operations
	"EQUALS", "NOT_EQUALS", "GE", "LE", "GREATER", "LESS", "SET",
	"MOD", "DIVIDE", "MULTIPLY", "MINUS", "PLUS",

	// WITH ARGS
	"IDENT",
	"UINT",
	"STR",
};

void token_expect_type(token_t token, token_type_t token_type) {
	if (token->type != token_type) {
		printf("ERROR: expected %s, found %s\n", str_token[token_type], str_token[token->type]);
		exit(1);
	}
}

token_t tokens_try_get(tokens_slice_t *slice, size_t idx) {
	if (idx >= slice->len) {
		printf("ERROR: trying to get %d token when its %d tokes total\n", idx, slice->len);
		exit(1);
	}
    return slice_at(token_t, slice, idx);
}

tokens_slice_t tokens_get_circle_scope(tokens_slice_t *slice, size_t from) {
	size_t i = from;
	size_t level = 0;
	token_t token = slice_at(token_t, slice, i);
	assert(token->type == TOKEN_OPENING_CIRCLE_BRACE);
	do {
		token = slice_at(token_t, slice, i);
		if (token->type == TOKEN_OPENING_CIRCLE_BRACE) {
			level++;
		} else if (token->type == TOKEN_CLOSING_CIRCLE_BRACE) {
			level--;
		}
		i++;
	} while (level != 0 && i < slice->len);
	if (token->type != TOKEN_CLOSING_CIRCLE_BRACE) {
		printf("ERROR: scope was not closed!\n");
		exit(1);
	}
	tokens_slice_t after = subslice_after(slice, from + 1);
	return subslice_before(&after, i - from - 2);
}

tokens_slice_t tokens_before(tokens_slice_t *slice, token_type_t type) {
	size_t i = 0;
	for (; i < slice->len && slice_at(token_t, slice, i)->type != type; i++);
	return subslice_before(slice, i);
}

tokens_slice_t tokens_before_circle_scoped_maybe_included(tokens_slice_t *slice, token_type_t type, bool include) {
	size_t i = 0;
	size_t level = 0;
	for (; i < slice->len; i++) {
		token_t token = slice_at(token_t, slice, i);
		if (token->type == TOKEN_OPENING_CIRCLE_BRACE) {
			level++;
		} else if (token->type == TOKEN_CLOSING_CIRCLE_BRACE) {
			level--;
		}
		if (level == 0 && token->type == type) {
			if (include) i++;
			break;
		}
	}
	return subslice_before(slice, i);
}

tokens_slice_t tokens_before_circle_scoped_included(tokens_slice_t *slice, token_type_t type) {
	return tokens_before_circle_scoped_maybe_included(slice, type, true);
}

tokens_slice_t tokens_before_circle_scoped(tokens_slice_t *slice, token_type_t type) {
	return tokens_before_circle_scoped_maybe_included(slice, type, false);
}

str_t token_extract_ident(token_t token) {
	token_expect_type(token, TOKEN_IDENT); 
	return ((token_ident_t*)token)->ident;
}