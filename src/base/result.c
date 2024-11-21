#include "result.h"

void result_push(result_t *result, message_t message) {
    SEALED_ASSERT_ALL_USED(message, 2);
    if (message->kind == MESSAGE_ERROR) {
        result->success = false;
    }
    arr_push(&result->messages, &message);
}

void error_dump(error_t error) {
	SEALED_ASSERT_ALL_USED(error, 14);
	switch (error->kind) {
		case ERROR_UNKNOWN_TYPE: {
			printf("unknown type for variable `");
			str_slice_dump(&error_as_unknown_type(error)->of, stdout);
			printf("`");
			break;
		}
		case ERROR_REDEFINITION: {
			printf("redefinition of symbol `");
			str_slice_dump(&error_as_redefinition(error)->of, stdout);
			printf("`");
			break;
		}
		case ERROR_INVALID_TYPE: {
			error_invalid_type_t *error_type = error_as_invalid_type(error);
			printf("invalid type: `");
			str_slice_dump(&error_type->expected.slice, stdout);
			printf("` for `");
			str_slice_dump(&error_type->of, stdout);
			printf("` found `");
			str_slice_dump(&error_type->found.slice, stdout);
			printf("`");
			break;
		}
		case ERROR_INVALID_IF_COND_TYPE: {
			error_invalid_if_cond_type_t *error_type = error_as_invalid_if_cond_type(error);
			printf("if cond must be `bool`, not `");
			str_slice_dump(&error_type->found.slice, stdout);
			printf("`");
			break;
		}
		case ERROR_INVALID_FUNC_ARGS_COUNT: {
			error_invalid_func_args_count_t *error_type = error_as_invalid_func_args_count(error);
			printf("invalid func args count passed: passed %d args for func `", error_type->found);
			str_slice_dump(&error_type->of, stdout);
			printf("` but got %d", error_type->expected);
			break;
		}
		case ERROR_EXPR_BINOP_INVALID_TYPES: {
			error_binop_invalid_types_t *error_type = error_as_binop_invalid_types(error);
			printf("invalid types in binop: `");
			str_slice_dump(&error_type->left.slice, stdout);
			printf("` <> `");
			str_slice_dump(&error_type->right.slice, stdout);
			printf("`");
			break;
		}
		case ERROR_UNDEFINED_SYMBOL: {
			printf("symbol `");
			str_slice_dump(&error_as_undefined_symbol(error)->name, stdout);
			printf("` is undefined");
			break;
		}
		default: {
			const char *descriptions[error_variants_count] = {
				"cannot parse token here",
				"EOF while trying to get token",
				"unexpected token",
				"scope was not closed",
				"missing binop in expression",
				"explicit binop in expression",
				"string literal was not closed",
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL
			};
			const char *description = descriptions[error->kind];
			if (description) {
				printf("%s", description);
			}
			break;
		}
	}
}

void message_dump(message_t message) {
	SEALED_ASSERT_ALL_USED(message, 2);
	str_slice_dump(&message->file, stdout);
	printf(":%d:%d: ", message->location.line + 1, message->location.character + 1);
	switch (message->kind) {
		case MESSAGE_ERROR: {
			printf("ERROR: ");
			error_t error = message_as_error(message)->error;
			error_dump(error);
			printf("\n");
			break;
		}
		case MESSAGE_WARNING: {
			SEALED_ASSERT_ALL_USED(warning, 1);
			const char *descriptions[] = {
				"not yet implemented!",
			};
			printf("WARNING: %s\n", descriptions[message_as_warning(message)->type]);
			break;
		}
	}
}

void result_print(result_t *result) {
	SLICE_FOREACH(&result->messages.slice, message_t, message, {
		message_dump(*message);
	});
}
