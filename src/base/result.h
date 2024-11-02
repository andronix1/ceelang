#pragma once

#include <stdbool.h>
#include "messages.h"

typedef struct {
    bool success;
    messages_t messages;
} result_t;

void result_push(result_t *result, message_t message);
void result_print(result_t *result);

static inline result_t result_new() {
    result_t result = {
        .messages = messages_new_with_cap(1),
        .success = true
    };
    return result;
}

static inline void *result_free(result_t *result) {
    arr_free(&result->messages);
}