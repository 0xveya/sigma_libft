#pragma once

#include <sigma/result.h>
#include <sigma/types.h>

#include <stdbool.h>

#if !defined(__linux__) || !defined(__x86_64__)
#error "sigma_sys currently supports Linux x86_64 only"
#endif

typedef iptr syscall_result_t;
typedef iptr syscall_arg_t;

typedef struct sigma_sys_error {
  i32 code;
} sigma_sys_error_t;

RESULT(sigma_sys_result, iptr, sigma_sys_error_t);
typedef sigma_sys_result sigma_sys_result_t;
RESULT(sigma_read_result, usize, sigma_sys_error_t);
typedef sigma_read_result sigma_read_result_t;
RESULT(sigma_write_result, usize, sigma_sys_error_t);
typedef sigma_write_result sigma_write_result_t;
RESULT(sigma_mmap_result, void *, sigma_sys_error_t);
typedef sigma_mmap_result sigma_mmap_result_t;
RESULT(sigma_munmap_result, sigma_unit_t, sigma_sys_error_t);
typedef sigma_munmap_result sigma_munmap_result_t;
RESULT(sigma_status_result, sigma_unit_t, sigma_sys_error_t);
typedef sigma_status_result sigma_status_result_t;

typedef enum sigma_arch_operation {
  SIGMA_ARCH_SET_FS = 0x1002,
} sigma_arch_operation_t;

typedef enum sigma_mmap_protection {
  SIGMA_PROT_NONE = 0,
  SIGMA_PROT_READ = 1 << 0,
  SIGMA_PROT_WRITE = 1 << 1,
  SIGMA_PROT_EXEC = 1 << 2,
} sigma_mmap_protection_t;

typedef enum sigma_mmap_flags {
  SIGMA_MAP_SHARED = 1 << 0,
  SIGMA_MAP_PRIVATE = 1 << 1,
  SIGMA_MAP_FIXED = 1 << 4,
  SIGMA_MAP_ANONYMOUS = 1 << 5,
} sigma_mmap_flags_t;

#include <sigma/sys/linux/syscalls.def.h>

#define SIGMA_SYS_ARG_DECL(arg) SIGMA_SYS_ARG_DECL_I arg
#define SIGMA_SYS_ARG_DECL_I(type, name) type name
#define SIGMA_SYS_DECLS_0(...) void
#define SIGMA_SYS_DECLS_1(a0) SIGMA_SYS_ARG_DECL(a0)
#define SIGMA_SYS_DECLS_2(a0, a1) SIGMA_SYS_DECLS_1(a0), SIGMA_SYS_ARG_DECL(a1)
#define SIGMA_SYS_DECLS_3(a0, a1, a2)                                          \
  SIGMA_SYS_DECLS_2(a0, a1), SIGMA_SYS_ARG_DECL(a2)
#define SIGMA_SYS_DECLS_4(a0, a1, a2, a3)                                      \
  SIGMA_SYS_DECLS_3(a0, a1, a2), SIGMA_SYS_ARG_DECL(a3)
#define SIGMA_SYS_DECLS_5(a0, a1, a2, a3, a4)                                  \
  SIGMA_SYS_DECLS_4(a0, a1, a2, a3), SIGMA_SYS_ARG_DECL(a4)
#define SIGMA_SYS_DECLS_6(a0, a1, a2, a3, a4, a5)                              \
  SIGMA_SYS_DECLS_5(a0, a1, a2, a3, a4), SIGMA_SYS_ARG_DECL(a5)
#define SIGMA_SYS_CAT_I(left, right) left##right
#define SIGMA_SYS_CAT(left, right) SIGMA_SYS_CAT_I(left, right)
#define SIGMA_SYS_DECLS(count, ...)                                            \
  SIGMA_SYS_CAT(SIGMA_SYS_DECLS_, count)(__VA_ARGS__)

#define SIGMA_DECLARE_NORMAL(nr, name, argc, kind, type, policy, ...)          \
  type s_##name(SIGMA_SYS_DECLS(argc, __VA_ARGS__));
#define SIGMA_DECLARE_NORETURN(nr, name, argc, kind, type, policy, ...)        \
  [[noreturn]] void s_##name(SIGMA_SYS_DECLS(argc, __VA_ARGS__));
#define X(nr, name, argc, kind, type, policy, ...)                             \
  SIGMA_SYS_CAT(SIGMA_DECLARE_, kind)(nr, name, argc, kind, type, policy,      \
                                      __VA_ARGS__)

SIGMA_LINUX_SYSCALLS(X)

#undef X
