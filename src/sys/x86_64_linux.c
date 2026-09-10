#include <sigma/sys.h>

enum {
  LINUX_SYS_WRITE = 1,
  LINUX_SYS_MMAP = 9,
  LINUX_SYS_MUNMAP = 11,
  LINUX_SYS_EXIT = 60,
};

static bool linux_syscall_failed(long value) {
  return (unsigned long)value >= (unsigned long)-4095;
}

static long linux_syscall1(long number, long arg0) {
  long result;
  __asm__ volatile("syscall"
                   : "=a"(result)
                   : "a"(number), "D"(arg0)
                   : "rcx", "r11", "memory");
  return result;
}

static long linux_syscall2(long number, long arg0, long arg1) {
  long result;
  __asm__ volatile("syscall"
                   : "=a"(result)
                   : "a"(number), "D"(arg0), "S"(arg1)
                   : "rcx", "r11", "memory");
  return result;
}

static long linux_syscall3(long number, long arg0, long arg1, long arg2) {
  long result;
  __asm__ volatile("syscall"
                   : "=a"(result)
                   : "a"(number), "D"(arg0), "S"(arg1), "d"(arg2)
                   : "rcx", "r11", "memory");
  return result;
}

static long linux_syscall6(long number, long arg0, long arg1, long arg2,
                           long arg3, long arg4, long arg5) {
  register long r10 __asm__("r10") = arg3;
  register long r8 __asm__("r8") = arg4;
  register long r9 __asm__("r9") = arg5;
  long result;
  __asm__ volatile("syscall"
                   : "=a"(result)
                   : "a"(number), "D"(arg0), "S"(arg1), "d"(arg2), "r"(r10),
                     "r"(r8), "r"(r9)
                   : "rcx", "r11", "memory");
  return result;
}

/* sigma:begin
name: sys.write
provides: sys.write
deps:
externals: linux.x86_64.syscall
kind: provider
*/
sigma_write_result_t s_write(int fd, const void *data, sigma_sys_usize size) {
  long raw = linux_syscall3(LINUX_SYS_WRITE, fd, (long)data, (long)size);
  if (linux_syscall_failed(raw))
    return (sigma_write_result_t){.ok = false, .error = {.code = (int)-raw}};
  return (sigma_write_result_t){.ok = true, .value = (sigma_sys_usize)raw};
}
/* sigma:end */

/* sigma:begin
name: sys.mmap
provides: sys.mmap
deps:
externals: linux.x86_64.syscall
kind: provider
*/
sigma_mmap_result_t s_mmap(void *address, sigma_sys_usize size,
                           sigma_mmap_protection_t protection,
                           sigma_mmap_flags_t flags, int fd,
                           sigma_sys_usize offset) {
  long raw = linux_syscall6(LINUX_SYS_MMAP, (long)address, (long)size,
                            protection, flags, fd, (long)offset);
  if (linux_syscall_failed(raw))
    return (sigma_mmap_result_t){.ok = false, .error = {.code = (int)-raw}};
  return (sigma_mmap_result_t){.ok = true, .value = (void *)raw};
}
/* sigma:end */

/* sigma:begin
name: sys.munmap
provides: sys.munmap
deps:
externals: linux.x86_64.syscall
kind: provider
*/
sigma_munmap_result_t s_munmap(void *address, sigma_sys_usize size) {
  long raw = linux_syscall2(LINUX_SYS_MUNMAP, (long)address, (long)size);
  if (linux_syscall_failed(raw))
    return (sigma_munmap_result_t){.ok = false, .error = {.code = (int)-raw}};
  return (sigma_munmap_result_t){.ok = true};
}
/* sigma:end */

/* sigma:begin
name: sys.exit
provides: sys.exit
deps:
externals: linux.x86_64.syscall
kind: provider
*/
[[noreturn]] void s_exit(int status) {
  (void)linux_syscall1(LINUX_SYS_EXIT, status);
  __builtin_unreachable();
}
/* sigma:end */
