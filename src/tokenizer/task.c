#include "task.h"

void token_dump(token_t token) {
	SEALED_ASSERT_ALL_USED(token, 29);
	static const char *tokens_str[token_variants_count] = {
		";", ",", ":",
		"const", "var", "fun", "=",
		"return",
		"if", "elif", "else",
		"{", "}",
		"(", ")",
		"==", "!=", ">=", "<=", ">", "<",
		"/", "*", "-", "+", "%",
	};
	if (token->kind == TOKEN_IDENT) {
		str_slice_dump(&token_as_ident(token)->ident.slice, stdout);
	} else if (token->kind == TOKEN_STR) {
		fputc('\"', stdout);
		str_slice_dump(&token_as_str(token)->value.slice, stdout);
		fputc('\"', stdout);
	}  else if (token->kind == TOKEN_UINT) {
		printf("%d", token_as_uint(token)->value);
	} else {
		printf("%s", tokens_str[token->kind]);
	}
}

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

	result_t result = result_new();
	tokens_t tokens = tokens_new_with_cap(1);
	str_slice_t file_path_slice = str_slice_from_cstr(file_path);
	message_base_t message_base = message_base_new_simple(str_copy_from_slice(&file_path_slice), location_new(0, 0));
	tokenize(src, message_base, &result, &tokens);

	result_print(&result);
	printf("found tokens:\n");
	SLICE_FOREACH(&tokens.slice, token_t, token, {
		token_dump(*token);
		fputc(' ', stdout);
	});
	printf("\n-------------\n");

	if (result.success) {
		printf("Tokenizing finished!\n");
	} else {
		printf("Tokenizing failed!\n");
	}

	message_free_base(&message_base);
	result_free(&result);
	arr_free(&tokens);

	fclose(src);
	return TASK_OK;
}
