#include "instr.h"

SEALED_CHILD_IMPL(vm_instr, pushs);
SEALED_CHILD_IMPL(vm_instr, pushi);
SEALED_CHILD_IMPL(vm_instr, call);

ARR_TYPED_ALIAS_IMPL(vm_bc_builder, vm_instr_t, NULL);
