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

void token_free_wrap(token_t *token) {
    token_free(*token);
}

ARR_TYPED_ALIAS_IMPL(tokens, token_t, token_free_wrap);

token_get_scope_err_t token_try_get_scope(tokens_slice_t *slice, tokens_slice_t *dst, token_type_t opening, token_type_t closing) {
    if (slice->len == 0 || (*tokens_slice_at(slice, 0))->kind != opening) {
        return TOKEN_GET_SCOPE_NOT_STARTED;
    }
    size_t len = 1;
    size_t level = 1;
    while (len < slice->len) {
        token_type_t type = (*tokens_slice_at(slice, len++))->kind;
        if (type == opening) {
            level++;
        } else if (type == closing) {
            level--;
            if (level == 0) {
                *dst = subslice_range(slice, 1, len - 1);
                return TOKEN_GET_SCOPE_OK;
            }
        }
    }
    return TOKEN_GET_SCOPE_NOT_CLOSED;
}

tokens_slice_t tokens_before(tokens_slice_t *slice, token_type_t type) {
    size_t i = 0;
    for (i = 0; i < slice->len; i++) {
        token_type_t src_type = (*tokens_slice_at(slice, i))->kind;
        if (src_type == type) {
            return subslice_before(slice, i);
        }
    }
    return *slice;
}

tokens_slice_t tokens_before_scoped(tokens_slice_t *slice, token_type_t stop_type, token_type_t opening, token_type_t closing) {
    if (slice->len == 0) {
        return *slice;
    }
    size_t len = 0;
    size_t level = 0;
    while (len < slice->len) {
        token_type_t type = (*tokens_slice_at(slice, len++))->kind;
        if (level == 0 && type == stop_type) {
            return subslice_before(slice, len - 1);
        }
        if (type == opening) {
            level++;
        } else if (type == closing) {
            if (level == 0) {
                return *slice;
            }
            level--;
        }
    }
    return *slice;
}