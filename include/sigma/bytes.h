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

/* Returns true when the view contains no bytes. */
bool bytes_is_empty(bytes_t bytes);

/* Views */

/* Returns a clamped immutable subview without allocating. */
bytes_t bytes_sub(bytes_t bytes, usize start, usize len);

/* Returns a clamped mutable subview without allocating. */
bytes_mut_t bytes_mut_sub(bytes_mut_t bytes, usize start, usize len);

/* Comparison */

/* Compares both views lexicographically as unsigned bytes. */
i32 bytes_cmp(bytes_t a, bytes_t b);
/* Returns true when both views contain exactly the same bytes. */
bool bytes_eq(bytes_t a, bytes_t b);

/* Search */

/* Returns the first matching offset, or SIGMA_NPOS when absent. */
usize bytes_find(bytes_t bytes, u8 value);
/* Returns the final matching offset, or SIGMA_NPOS when absent. */
usize bytes_rfind(bytes_t bytes, u8 value);
