#include "messages.h"

SEALED_BASE_IMPL_FREE(message, { arr_free(&self->file); });
SEALED_FREE_IMPL_WITH_BASE_FREE(message, {}, 2);

SEALED_CHILD_IMPL(message, error);
SEALED_CHILD_IMPL(message, warning);

ARR_TYPED_ALIAS_IMPL(messages, message_t, message_free);