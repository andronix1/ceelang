#include "char_ext.h"

bool is_whitespace(char c) {
	return c == ' ' || c == '\t';
}

bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

bool is_lowercase(char c) {
	return c >= 'a' && c <= 'z';
}

bool is_uppercase(char c) {
	return c >= 'A' && c <= 'Z';
}

bool is_latin(char c) {
	return is_lowercase(c) || is_uppercase(c);
}

bool is_start_ident(char c) {
	return is_latin(c) || c == '_' || c == '@';
}

bool is_ident(char c) {
	return is_start_ident(c) || is_digit(c);
}
