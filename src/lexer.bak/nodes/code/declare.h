#pragma once

typedef enum {
	DECLARE_CONST,
	DECLARE_VAR
} declare_type_t;

typedef struct {
	stat_type_t type = STAT_DECLARE;
	declare_type_t declare_type;
	ident_t name;
	ident_t type; // OPTIONAL
	expr_t expr; // OPTIONAL
} stat_declare_t;


