#include <sigma/sys.h>

#include "syscall_macros.h"
#include "x86_64_linux_abi.def.h"

#define X(nr, name, argc, kind, type, policy, ...) LINUX_SYS_##name = nr,
enum { SIGMA_LINUX_SYSCALLS(X) };
#undef X

static bool linux_syscall_failed(syscall_result_t value) {
  return (uptr)value >= (uptr)-4095;
}

#define SIGMA_DEFINE_RAW_NORMAL(argc)                                          \
  static inline __attribute__((unused)) syscall_result_t linux_syscall##argc(  \
      syscall_result_t number, SIGMA_X86_64_PARAMS_##argc) {                   \
    SIGMA_X86_64_REGISTERS_##argc syscall_result_t result;                     \
    __asm__ volatile("syscall"                                                 \
                     : "=a"(result)                                            \
                     : "a"(number), SIGMA_X86_64_INPUTS_##argc                 \
                     : "rcx", "r11", "memory");                                \
    return result;                                                             \
  }
#define SIGMA_DEFINE_RAW_ZERO(argc)                                            \
  static inline __attribute__((unused)) syscall_result_t linux_syscall##argc(  \
      syscall_result_t number) {                                               \
    syscall_result_t result;                                                   \
    __asm__ volatile("syscall"                                                 \
                     : "=a"(result)                                            \
                     : "a"(number)                                             \
                     : "rcx", "r11", "memory");                                \
    return result;                                                             \
  }

#define X(argc, kind) SIGMA_SYS_CAT(SIGMA_DEFINE_RAW_, kind)(argc)
SIGMA_X86_64_SYSCALL_ARITIES(X)
#undef X

#define SIGMA_LINUX_SYSCALL_0(number, ...) linux_syscall0(number)
#define SIGMA_LINUX_SYSCALL_1(number, ...) linux_syscall1(number, __VA_ARGS__)
#define SIGMA_LINUX_SYSCALL_2(number, ...) linux_syscall2(number, __VA_ARGS__)
#define SIGMA_LINUX_SYSCALL_3(number, ...) linux_syscall3(number, __VA_ARGS__)
#define SIGMA_LINUX_SYSCALL_4(number, ...) linux_syscall4(number, __VA_ARGS__)
#define SIGMA_LINUX_SYSCALL_5(number, ...) linux_syscall5(number, __VA_ARGS__)
#define SIGMA_LINUX_SYSCALL_6(number, ...) linux_syscall6(number, __VA_ARGS__)
#define SIGMA_LINUX_SYSCALL(argc, number, ...)                                 \
  SIGMA_SYS_CAT(SIGMA_LINUX_SYSCALL_, argc)(number, __VA_ARGS__)

#define SIGMA_RESULT_USIZE(type, raw)                                          \
  do {                                                                         \
    type sigma_result__;                                                       \
    sigma_result__.ok = !linux_syscall_failed(raw);                            \
    if (sigma_result__.ok)                                                     \
      sigma_result__.value = (usize)(raw);                                     \
    else                                                                       \
      sigma_result__.error.code = (i32) - (raw);                               \
    return sigma_result__;                                                     \
  } while (false)

#define SIGMA_RESULT_POINTER(type, raw)                                        \
  do {                                                                         \
    type sigma_result__;                                                       \
    sigma_result__.ok = !linux_syscall_failed(raw);                            \
    if (sigma_result__.ok)                                                     \
      sigma_result__.value = (void *)(raw);                                    \
    else                                                                       \
      sigma_result__.error.code = (i32) - (raw);                               \
    return sigma_result__;                                                     \
  } while (false)

#define SIGMA_RESULT_STATUS(type, raw)                                         \
  do {                                                                         \
    type sigma_result__;                                                       \
    sigma_result__.ok = !linux_syscall_failed(raw);                            \
    sigma_result__.error.code = sigma_result__.ok ? 0 : (i32) - (raw);         \
    return sigma_result__;                                                     \
  } while (false)

#define SIGMA_RESULT_IPTR(type, raw)                                           \
  do {                                                                         \
    type sigma_result__;                                                       \
    sigma_result__.ok = !linux_syscall_failed(raw);                            \
    if (sigma_result__.ok)                                                     \
      sigma_result__.value = (iptr)(raw);                                      \
    else                                                                       \
      sigma_result__.error.code = (i32) - (raw);                               \
    return sigma_result__;                                                     \
  } while (false)

#define SIGMA_DEFINE_NORMAL(nr, name, argc, kind, type, policy, ...)           \
  type s_##name(SIGMA_SYS_DECLS(argc, __VA_ARGS__)) {                          \
    syscall_result_t raw = SIGMA_LINUX_SYSCALL(                                \
        argc, LINUX_SYS_##name, SIGMA_SYS_VALUES(argc, __VA_ARGS__));          \
    SIGMA_RESULT_##policy(type, raw);                                          \
  }

#define SIGMA_DEFINE_NORETURN(nr, name, argc, kind, type, policy, ...)         \
  [[noreturn]] void s_##name(SIGMA_SYS_DECLS(argc, __VA_ARGS__)) {             \
    (void)SIGMA_LINUX_SYSCALL(argc, LINUX_SYS_##name,                          \
                              SIGMA_SYS_VALUES(argc, __VA_ARGS__));            \
    __builtin_unreachable();                                                   \
  }

#define X(nr, name, argc, kind, type, policy, ...)                             \
  SIGMA_SYS_CAT(SIGMA_DEFINE_, kind)(nr, name, argc, kind, type, policy,       \
                                     __VA_ARGS__)

SIGMA_LINUX_SYSCALLS(X)

#undef X
