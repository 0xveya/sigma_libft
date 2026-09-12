#pragma once

#define SIGMA_SYS_ARG_VALUE(arg) SIGMA_SYS_ARG_VALUE_I arg
#define SIGMA_SYS_ARG_VALUE_I(type, name) ((syscall_arg_t)(name))

#define SIGMA_SYS_VALUES_0(...)

#define SIGMA_SYS_VALUES_1(a0) SIGMA_SYS_ARG_VALUE(a0)

#define SIGMA_SYS_VALUES_2(a0, a1)                                             \
  SIGMA_SYS_ARG_VALUE(a0), SIGMA_SYS_ARG_VALUE(a1)

#define SIGMA_SYS_VALUES_3(a0, a1, a2)                                         \
  SIGMA_SYS_ARG_VALUE(a0), SIGMA_SYS_ARG_VALUE(a1), SIGMA_SYS_ARG_VALUE(a2)

#define SIGMA_SYS_VALUES_4(a0, a1, a2, a3)                                     \
  SIGMA_SYS_ARG_VALUE(a0), SIGMA_SYS_ARG_VALUE(a1), SIGMA_SYS_ARG_VALUE(a2),   \
      SIGMA_SYS_ARG_VALUE(a3)

#define SIGMA_SYS_VALUES_5(a0, a1, a2, a3, a4)                                 \
  SIGMA_SYS_ARG_VALUE(a0), SIGMA_SYS_ARG_VALUE(a1), SIGMA_SYS_ARG_VALUE(a2),   \
      SIGMA_SYS_ARG_VALUE(a3), SIGMA_SYS_ARG_VALUE(a4)

#define SIGMA_SYS_VALUES_6(a0, a1, a2, a3, a4, a5)                             \
  SIGMA_SYS_ARG_VALUE(a0), SIGMA_SYS_ARG_VALUE(a1), SIGMA_SYS_ARG_VALUE(a2),   \
      SIGMA_SYS_ARG_VALUE(a3), SIGMA_SYS_ARG_VALUE(a4),                        \
      SIGMA_SYS_ARG_VALUE(a5)

#define SIGMA_SYS_VALUES(n, ...)                                               \
  SIGMA_SYS_CAT(SIGMA_SYS_VALUES_, n)(__VA_ARGS__)
