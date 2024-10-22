#pragma once

#include "expr.h"
#include "lexer/base.h"

typedef enum {
	STAT_ASSIGN,
	STAT_DECLARE,
	STAT_COND,
} stat_type_t;

typedef struct {
	stat_type_t type;
} stat_base_t;

// ASSIGN

typedef struct {
	stat_type_t type = STAT_ASSIGN;
	ident_t name;
	expr_t expr;
} stat_assign_t;

// DECLARE

typedef enum {
	DECLARE_CONST,
	DECLARE_VAR
} declare_type_t;

typedef struct {
	stat_type_t type = STAT_DECLARE;
	declare_type_t declare_type;
	ident_t name;
	expr_t expr; // OPTIONAL
} stat_declare_t;

// IF

typedef struct {
	stat_type_t type = STAT_IF;
	expr_t condition;
	ast_block_node_t on_true;
	ast_block_node_t on_else; // OPTIONAL
} stat_if_t;

// --------

typedef stat_base_t* stat_t;

typedef struct {
	ast_node_type_t type = AST_NODE_BLOCK;
	ast_nodes_t nodes;
} ast_node_block_t;

typedef ast_node_base_t* ast_node_t;

