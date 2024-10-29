#include "tokens.h"

const char *str_token[TOKENS_COUNT] = {
	// META
	"SEMICOLON",
	// WITHOUT ARGS
	"CONST",
	"VAR",
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

str_t token_extract_ident(token_t token) {
	token_expect_type(token, TOKEN_IDENT); 
	return ((token_ident_t*)token)->ident;
}