#include "lexer/modules/scopes/scope.h"

#define TAB() for (size_t j = 0; j < level; j++) { printf("    "); }

void print_expr(expr_t expr);
void print_funcall(funcall_t *funcall);
void print_const(const_t data);
void print_expr(expr_t expr);
void print_definition(definition_t *definition, str_t *name);
void print_stat(stat_t stat, size_t level);
void print_stats(stats_slice_t *stats, size_t level);
void print_def(def_t def);
void print_scope(scope_t *scope);