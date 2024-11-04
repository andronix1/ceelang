#include "token_reader.h"

token_t token_next(size_t *len, tokens_slice_t *tokens, message_base_t *base) {
    token_t token = *tokens_slice_at(tokens, (*len)++);
    base->location = token->location;
    return token;
}