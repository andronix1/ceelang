#pragma once

static inline bool is_whitespace(char c) { return c == ' ' || c == '\t'; }
static inline bool is_digit(char c) { return c >= '0' && c <= '9'; }
static inline bool is_lowercase(char c) { return c >= 'a' && c <= 'z'; }
static inline bool is_uppercase(char c) { return c >= 'A' && c <= 'Z'; }
static inline bool is_latin(char c) { return is_lowercase(c) || is_uppercase(c); }

static inline bool is_start_ident(char c) { return is_latin(c) || c == '_'; }
static inline bool is_ident(char c) { return is_start_ident(c) || is_digit(c); }
