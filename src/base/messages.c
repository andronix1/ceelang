#include "messages.h"

SEALED_FREE_IMPL(message, {}, 2);

SEALED_CHILD_IMPL(message, error);
SEALED_CHILD_IMPL(message, warning);

void message_free_wrap(message_t *message) {
    message_free(*message);
}

SEALED_CHILD_IMPL(error, unknown_type);
ARR_TYPED_ALIAS_IMPL(messages, message_t, message_free_wrap);
