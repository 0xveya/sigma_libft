#pragma once

#include <sigma/qol.h>

#include <stdbool.h>

typedef struct {
  const u8 *items;
  usize len;
} bytes_t;

typedef struct {
  u8 *items;
  usize len;
} bytes_mut_t;

/* Properties */

bool bytes_is_empty(bytes_t bytes);

/* Views */

bytes_t bytes_sub(bytes_t bytes, usize start, usize len);

bytes_mut_t bytes_mut_sub(bytes_mut_t bytes, usize start, usize len);

/* Comparison */

i32 bytes_cmp(bytes_t a, bytes_t b);
bool bytes_eq(bytes_t a, bytes_t b);

/* Search */

usize bytes_find(bytes_t bytes, u8 value);
usize bytes_rfind(bytes_t bytes, u8 value);
