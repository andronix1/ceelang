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
	switch (message->kind) {
		case MESSAGE_ERROR: {
			printf("ERROR: %d\n", message_as_error(message)->type);
			break;
		}
		case MESSAGE_WARNING: {
			printf("WARNING: %d\n", message_as_warning(message)->type);
			break;
		}
	}
}

void result_print(result_t *result) {
	SLICE_FOREACH(&result->messages.slice, message_t, message, {
		message_dump(*message);
	});
}