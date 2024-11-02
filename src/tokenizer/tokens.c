#include "tokens.h"

SEALED_CHILD_IMPL_FREE(token, ident, {
    str_free(&self->ident);
});

SEALED_CHILD_IMPL_FREE(token, str, {
    str_free(&self->value);
});

SEALED_CHILD_IMPL(token, uint);

SEALED_FREE_IMPL(token, {
    SEALED_SHOULD_BE_FREED(token, TOKEN_IDENT, ident)
    SEALED_SHOULD_BE_FREED(token, TOKEN_STR, str)
}, 29);

ARR_TYPED_ALIAS_IMPL(tokens, token_t, token_free);