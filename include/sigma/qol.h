#pragma once

#include <sigma/sys.h>
#include <sigma/types.h>
#include <stdbool.h>
#define let auto
#define SIGMA_NPOS ((usize) - 1)

[[noreturn]] static inline void panic(const char *msg) {
  static const char prefix[] = "PANIC: ";
  usize len = 0;
  while (msg[len] != '\0')
    len++;
  (void)s_write(2, prefix, sizeof(prefix) - 1);
  (void)s_write(2, msg, len);
  (void)s_write(2, "\n", 1);
  s_exit(127);
}
