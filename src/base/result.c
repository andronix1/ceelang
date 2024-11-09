#include "result.h"

void result_push(result_t *result, message_t message) {
    SEALED_ASSERT_ALL_USED(message, 2);
    if (message->kind == MESSAGE_ERROR) {
        result->success = false;
    }
    arr_push(&result->messages, &message);
}

void message_dump(message_t message) {
	SEALED_ASSERT_ALL_USED(message, 2);
	str_slice_dump(&message->file, stdout);
	printf(":%d:%d: ", message->location.line + 1, message->location.character + 1);
	switch (message->kind) {
		case MESSAGE_ERROR: {
			SEALED_ASSERT_ALL_USED(error, 10);
			const char *descriptions[] = {
				"cannot parse token here",
				"EOF while trying to get token",
				"unexpected token",
				"scope was not closed",
				"missing binop in expression",
				"explicit binop in expression",
				"string literal was not closed",
				"unknown type for variable ",
				"redefinition of symbol ",
				"invalid type: "
			};
			error_t error = message_as_error(message)->error;
			printf("ERROR: %s", descriptions[error->kind]);
			if (error->kind == ERROR_UNKNOWN_TYPE) {
				printf("`");
				str_slice_dump(&error_as_unknown_type(error)->of, stdout);
				printf("`");
			} else if (error->kind == ERROR_REDEFINITION) {
				printf("`");
				str_slice_dump(&error_as_redefinition(error)->of, stdout);
				printf("`");
			} else if (error->kind == ERROR_INVALID_TYPE) {
				error_invalid_type_t *error_type = error_as_invalid_type(error);
				printf("expected `");
				str_slice_dump(&error_type->expected.slice, stdout);
				printf("` for `");
				str_slice_dump(&error_type->of, stdout);
				printf("` found `");
				str_slice_dump(&error_type->found.slice, stdout);
				printf("`");
			}
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
