#pragma once

typedef struct {
	stat_type_t type = STAT_IF;
	expr_t condition;
	ast_block_node_t on_true;
	ast_block_node_t on_else; // OPTIONAL
} stat_if_t;

