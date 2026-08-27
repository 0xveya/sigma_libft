#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#define var auto
#define SIGMA_NPOS ((usize) - 1)

typedef size_t usize;
typedef ptrdiff_t isize;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;
typedef uintptr_t uptr;
typedef intptr_t iptr;

_Noreturn static inline void panic(const char *msg) {
  usize len = 0;

  while (msg[len] != '\0')
    len++;
  (void)write(STDERR_FILENO, "PANIC: ", 7);
  (void)write(STDERR_FILENO, msg, len);
  (void)write(STDERR_FILENO, "\n", 1);
  __builtin_trap();
}
