#pragma once

#include <malloc.h>
#include <assert.h>
#include "core/macro.h"

// names
#define _SEALED_KIND_NAME(name) name##_type_t
#define _SEALED_KIND_VARIANTS_NAME(name) name##_variants_count
#define _SEALED_BASE_NAME(name) name##_base_t
#define _SEALED_FREE(name) name##_free
#define _SEALED_BASE_NEW(name) name##_base_new
#define _SEALED_BASE_NEW_SIMPLE(name) name##_base_new_simple
#define _SEALED_BASE_FREE_BASE(name) name##_free_base
#define _SEALED_CHILD_NAME(name, child_name) name##_##child_name##_t
#define _SEALED_CHILD_CAST(name, child_name) name##_as_##child_name
#define _SEALED_CHILD_NEW(name, child_name) name##_##child_name##_new
#define _SEALED_CHILD_FREE(name, child_name) name##_##child_name##_free

// params
#define SEALED_PARAM(type, name) (type, name)
#define _SEALED_TYPED(type, name) type name
#define _SEALED_NAME(type, name) name

// utils
#define _SEALED_PARAM(value) _SEALED_TYPED value;
#define _SEALED_PARAMS(...) FOREACH(_SEALED_PARAM, ##__VA_ARGS__)

#define _SEALED_ARG(value) _SEALED_TYPED value, 
#define _SEALED_ARG_LAST(value) _SEALED_TYPED value
#define _SEALED_ARGS(...) FOREACH_WITH_LAST(_SEALED_ARG, _SEALED_ARG_LAST, ##__VA_ARGS__)

#define _SEALED_ARG_ASSIGN_TO_RESULT_PTR(value) result->_SEALED_NAME value = _SEALED_NAME value;
#define _SEALED_ARGS_ASSIGN_TO_RESULT_PTR(...) FOREACH(_SEALED_ARG_ASSIGN_TO_RESULT_PTR, ##__VA_ARGS__)

#define _SEALED_ARG_ASSIGN_TO_RESULT(value) result._SEALED_NAME value = _SEALED_NAME value;
#define _SEALED_ARGS_ASSIGN_TO_RESULT(...) FOREACH(_SEALED_ARG_ASSIGN_TO_RESULT, ##__VA_ARGS__)

#define _SEALED_CONSTRUCTOR_SIMPLE(type, const_name, ...) static inline type const_name(_SEALED_ARGS(__VA_ARGS__)) { type result; _SEALED_ARGS_ASSIGN_TO_RESULT(__VA_ARGS__) return result; }
#define _SEALED_CONSTRUCTOR(type, _kind, name, const_name, ...) static inline type *const_name(_SEALED_ARGS(__VA_ARGS__)) { type *result = malloc(sizeof(type)); _SEALED_ARGS_ASSIGN_TO_RESULT_PTR(__VA_ARGS__); ((name##_t)result)->kind = _kind; return result; }


// kind
#define SEALED_KIND(name, ...) typedef enum { __VA_ARGS__, _SEALED_KIND_VARIANTS_NAME(name) } _SEALED_KIND_NAME(name)

// base
#define _SEALED_BASE_STRUCT(name, ...) typedef struct { _SEALED_KIND_NAME(name) kind; _SEALED_PARAMS(__VA_ARGS__) } _SEALED_BASE_NAME(name)
#define _SEALED_BASE_TYPEDEF(name) typedef _SEALED_BASE_NAME(name)* name##_t
#define SEALED_BASE(name, ...) \
    _SEALED_BASE_STRUCT(name, ##__VA_ARGS__); \
    _SEALED_BASE_TYPEDEF(name); \
    _SEALED_CONSTRUCTOR_SIMPLE(_SEALED_BASE_NAME(name), _SEALED_BASE_NEW_SIMPLE(name), ##__VA_ARGS__); \
    static inline name##_t _SEALED_BASE_NEW(name)(_SEALED_ARGS(SEALED_PARAM(_SEALED_KIND_NAME(name), kind), ##__VA_ARGS__)) { _SEALED_BASE_NAME(name) *result = malloc(sizeof(_SEALED_BASE_NAME(name))); _SEALED_ARGS_ASSIGN_TO_RESULT_PTR(SEALED_PARAM(_SEALED_KIND_NAME(name), kind), ##__VA_ARGS__); return result; }

// base funcs
#define SEALED_BASE_DEFINE_FREE(name) void _SEALED_BASE_FREE_BASE(name)(_SEALED_BASE_NAME(name) *self)
#define SEALED_BASE_IMPL_FREE(name, body) SEALED_BASE_DEFINE_FREE(name) body

// child
#define _SEALED_CHILD_STRUCT(name, child_name, ...) typedef struct { _SEALED_BASE_NAME(name) base; _SEALED_PARAMS(__VA_ARGS__) } _SEALED_CHILD_NAME(name, child_name)
#define _SEALED_CHILD_CAST_DEF(name, child_name) _SEALED_CHILD_NAME(name, child_name) *_SEALED_CHILD_CAST(name, child_name)(name##_t base);
#define SEALED_CHILD_DEFINE(name, kind, child_name, ...) \
    _SEALED_CHILD_STRUCT(name, child_name, ##__VA_ARGS__); \
    _SEALED_CHILD_CAST_DEF(name, child_name) \
    _SEALED_CONSTRUCTOR(_SEALED_CHILD_NAME(name, child_name), kind, name, _SEALED_CHILD_NEW(name, child_name), SEALED_PARAM(_SEALED_BASE_NAME(name), base), ##__VA_ARGS__)

#define SEALED_CHILD_IMPL(name, child_name) _SEALED_CHILD_NAME(name, child_name) *_SEALED_CHILD_CAST(name, child_name)(_SEALED_BASE_NAME(name) *base) { return (_SEALED_CHILD_NAME(name, child_name)*)base; }

#define SEALED_CHILD_DEFINE_FREE(name, kind, child_name, ...) SEALED_CHILD_DEFINE(name, kind, child_name, ##__VA_ARGS__); void _SEALED_CHILD_FREE(name, child_name)(_SEALED_CHILD_NAME(name, child_name) *self)
#define SEALED_CHILD_IMPL_FREE(name, child_name, body) SEALED_CHILD_IMPL(name, child_name); void _SEALED_CHILD_FREE(name, child_name)(_SEALED_CHILD_NAME(name, child_name) *self) body

#define SEALED_ASSERT_ALL_USED(name, used) static_assert(used == _SEALED_KIND_VARIANTS_NAME(name))

#define SEALED_FREE_DEFINE(name) void _SEALED_FREE(name)(_SEALED_BASE_NAME(name) *base)
#define SEALED_FREE_IMPL(name, body, used) void _SEALED_FREE(name)(_SEALED_BASE_NAME(name) *base) { SEALED_ASSERT_ALL_USED(name, used); switch (base->kind) body; free(base); }
#define SEALED_FREE_IMPL_WITH_BASE_FREE(name, body, used) void _SEALED_FREE(name)(_SEALED_BASE_NAME(name) *base) { SEALED_ASSERT_ALL_USED(name, used); switch (base->kind) body; _SEALED_BASE_FREE_BASE(name)(base); free(base); }
#define SEALED_SHOULD_BE_FREED(name, kind, child_name) case kind: _SEALED_CHILD_FREE(name, child_name)((_SEALED_CHILD_NAME(name, child_name)*)base); break;