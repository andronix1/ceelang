#include "task.h"

const char *tokens_str[TOKENS_COUNT] = {
	";\n",
	"const", "var", "fun", "if", "else",
	"{\n", "}\n",
	"(", ")",
	",", ":",
	"==", "!=", ">=", "<=", ">", "<", "=",
	"%", "/", "*", "-", "+",
	"<IDENT>",
	"<UINT>"
};

task_err_t tokens_task(int argc, char **argv) {
	const char *file_path = next_arg(&argc, &argv);
	if (!file_path) {
		printf("ERROR: missing source file path\n");
		return TASK_ERR_INVALID_USAGE;
	}
	
	FILE *src = fopen(file_path, "r");
	if (!src) {
		printf("ERROR: failed to open file: %s\n", strerror(errno));
		return TASK_ERR_INTERNAL;
	}

	tokens_t tokens = tokenize(src);

	if (arr_len(&tokens) == 0) {
		printf("no tokens available\n");
	} else {
		arr_foreach(token_t, &tokens, token, {
			token_type_t token_type = (*token)->type;
			switch (token_type) {
				case TOKEN_IDENT:
					token_ident_t *token_ident = (token_ident_t*)*token;
					str_push(&token_ident->ident, '\0');
					printf("%s ", token_ident->ident.slice.ptr);
					break;
				case TOKEN_UINT:
					token_uint_t *token_uint = (token_uint_t*)*token;
					printf("%d ", token_uint->value);
					break;
				default:
					printf("%s", tokens_str[(*token)->type]);
					if (
						token_type != TOKEN_SEMICOLON &&
						token_type != TOKEN_OPENING_FIGURE_BRACE &&
						token_type != TOKEN_CLOSING_FIGURE_BRACE 
					) {
						putc(' ', stdout);
					}
					break;
			}
		});
	}

	arr_free(&tokens);

	fclose(src);
	return TASK_OK;
}
