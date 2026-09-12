#pragma once

#define SIGMA_X86_64_SYSCALL_ARITIES(X)                                        \
  X(1)                                                                         \
  X(2)                                                                         \
  X(3)                                                                         \
  X(4)                                                                         \
  X(5)                                                                         \
  X(6)

#define SIGMA_X86_64_PARAMS_1 syscall_arg_t arg0
#define SIGMA_X86_64_PARAMS_2 SIGMA_X86_64_PARAMS_1, syscall_arg_t arg1
#define SIGMA_X86_64_PARAMS_3 SIGMA_X86_64_PARAMS_2, syscall_arg_t arg2
#define SIGMA_X86_64_PARAMS_4 SIGMA_X86_64_PARAMS_3, syscall_arg_t arg3
#define SIGMA_X86_64_PARAMS_5 SIGMA_X86_64_PARAMS_4, syscall_arg_t arg4
#define SIGMA_X86_64_PARAMS_6 SIGMA_X86_64_PARAMS_5, syscall_arg_t arg5

#define SIGMA_X86_64_INPUTS_1 "D"(arg0)
#define SIGMA_X86_64_INPUTS_2 SIGMA_X86_64_INPUTS_1, "S"(arg1)
#define SIGMA_X86_64_INPUTS_3 SIGMA_X86_64_INPUTS_2, "d"(arg2)
#define SIGMA_X86_64_INPUTS_4 SIGMA_X86_64_INPUTS_3, "r"(r10)
#define SIGMA_X86_64_INPUTS_5 SIGMA_X86_64_INPUTS_4, "r"(r8)
#define SIGMA_X86_64_INPUTS_6 SIGMA_X86_64_INPUTS_5, "r"(r9)

#define SIGMA_X86_64_REGISTERS_1
#define SIGMA_X86_64_REGISTERS_2
#define SIGMA_X86_64_REGISTERS_3
#define SIGMA_X86_64_REGISTERS_4                                               \
  register syscall_arg_t r10 __asm__("r10") = arg3;
#define SIGMA_X86_64_REGISTERS_5                                               \
  SIGMA_X86_64_REGISTERS_4                                                     \
  register syscall_arg_t r8 __asm__("r8") = arg4;
#define SIGMA_X86_64_REGISTERS_6                                               \
  SIGMA_X86_64_REGISTERS_5                                                     \
  register syscall_arg_t r9 __asm__("r9") = arg5;
