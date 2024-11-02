#pragma once

#include <stddef.h>

typedef struct {
    size_t line;
    size_t character;
} location_t;

static inline location_t location_new(size_t line, size_t character) {
    location_t result;
    result.line = line;
    result.character = character;
    return result;
}