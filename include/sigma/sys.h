#pragma once

#include <stdbool.h>
#include <stddef.h>

#if !defined(__linux__) || !defined(__x86_64__)
#error "sigma_sys currently supports Linux x86_64 only"
#endif

typedef size_t sigma_sys_usize;

typedef struct sigma_sys_error {
  int code;
} sigma_sys_error_t;

typedef struct sigma_write_result {
  bool ok;
  union {
    sigma_sys_usize value;
    sigma_sys_error_t error;
  };
} sigma_write_result_t;

typedef struct sigma_mmap_result {
  bool ok;
  union {
    void *value;
    sigma_sys_error_t error;
  };
} sigma_mmap_result_t;

typedef struct sigma_munmap_result {
  bool ok;
  sigma_sys_error_t error;
} sigma_munmap_result_t;

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

/* Writes bytes to fd and converts Linux negative errno returns into a tag. */
sigma_write_result_t s_write(int fd, const void *data, sigma_sys_usize size);
/* Maps pages and converts Linux negative errno returns into a tag. */
sigma_mmap_result_t s_mmap(void *address, sigma_sys_usize size,
                           sigma_mmap_protection_t protection,
                           sigma_mmap_flags_t flags, int fd,
                           sigma_sys_usize offset);
/* Unmaps pages and converts Linux negative errno returns into a tag. */
sigma_munmap_result_t s_munmap(void *address, sigma_sys_usize size);
/* Terminates the process without running userspace cleanup. */
[[noreturn]] void s_exit(int status);
