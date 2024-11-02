#include "args.h"

void func_arg_free(func_arg_t *arg) {
    str_free(&arg->name);
    str_free(&arg->type);
}

ARR_TYPED_ALIAS_IMPL(func_args, func_arg_t, func_arg_free)