#pragma once

#define SIGMA_LINUX_SYSCALLS(X)                                                \
  X(0, read, 3, NORMAL, sigma_read_result_t, USIZE, (i32, fd), (void *, data), \
    (usize, size))                                                             \
  X(1, write, 3, NORMAL, sigma_write_result_t, USIZE, (i32, fd),               \
    (const void *, data), (usize, size))                                       \
  X(9, mmap, 6, NORMAL, sigma_mmap_result_t, POINTER, (void *, address),       \
    (usize, size), (sigma_mmap_protection_t, protection),                      \
    (sigma_mmap_flags_t, flags), (i32, fd), (usize, offset))                   \
  X(11, munmap, 2, NORMAL, sigma_munmap_result_t, STATUS, (void *, address),   \
    (usize, size))                                                             \
  X(60, exit, 1, NORETURN, void, NONE, (i32, status))                          \
  X(158, arch_prctl, 2, NORMAL, sigma_status_result_t, STATUS,                 \
    (sigma_arch_operation_t, operation), (uptr, address))
