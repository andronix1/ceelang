#include "reader.h"

SEALED_CHILD_IMPL_FREE(token_read_result, ok, { });
SEALED_CHILD_IMPL(token_read_result, invalid);

SEALED_FREE_IMPL(token_read_result, {
    SEALED_SHOULD_BE_FREED(token_read_result, TOKEN_READ_OK, ok)
}, 3);