#include "messages.h"

SEALED_FREE_IMPL(message, {}, 2);

SEALED_CHILD_IMPL(message, error);
SEALED_CHILD_IMPL(message, warning);

void message_free_wrap(message_t *message) {
    message_free(*message);
}

SEALED_CHILD_IMPL(error, unknown_type);
SEALED_CHILD_IMPL(error, redefinition);
SEALED_CHILD_IMPL(error, invalid_type);
SEALED_CHILD_IMPL(error, invalid_if_cond_type);
SEALED_CHILD_IMPL(error, invalid_func_args_count);
SEALED_CHILD_IMPL(error, binop_invalid_types);
SEALED_CHILD_IMPL(error, undefined_symbol);
ARR_TYPED_ALIAS_IMPL(messages, message_t, message_free_wrap);
