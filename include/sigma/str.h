#pragma once

#include <allocator_vtable.h>
#include <sigma/attributes.h>
#include <sigma/bytes.h>
#include <sigma/qol.h>

#include <stdbool.h>

typedef struct {
  const char *items;
  usize len;
} str_t;

typedef struct {
  char *items;
  usize len;
} str_mut_t;

typedef struct {
  str_t remaining;
  u8 delimiter;
  bool finished;
} str_split_scalar_t;

/*
 * Compile-time string literal view.
 *
 * Only use with arrays/string literals:
 *
 *   STR_LIT("hello")     // correct
 *
 * Do not use with char *:
 *
 *   char *s = ...;
 *   STR_LIT(s);          // wrong: sizeof(s) is sizeof(char *)
 */
#define STR_LIT(s) ((str_t){.items = (s), .len = sizeof(s) - 1})

/* Construction / conversion */

str_t str_from_cstr(const char *s);

SIGMA_NODISCARD
char *str_to_cstr(str_t s, allocator_t allocator);

/* Properties */

bool str_is_empty(str_t s);

/* Views */

str_t str_sub(str_t s, usize start, usize len);

static inline bytes_t str_bytes(str_t s) {
  return (bytes_t){
      .items = (const u8 *)s.items,
      .len = s.len,
  };
}

static inline bytes_mut_t str_mut_bytes(str_mut_t s) {
  return (bytes_mut_t){
      .items = (u8 *)s.items,
      .len = s.len,
  };
}

/* Comparison */

i32 str_cmp(str_t a, str_t b);
bool str_eq(str_t a, str_t b);

bool str_ends_with(str_t s, str_t suffix);
bool str_starts_with(str_t s, str_t prefix);

/* Search */

usize str_find_byte(str_t s, u8 byte);
usize str_rfind_byte(str_t s, u8 byte);

/* Trimming */

str_t str_trim_ascii(str_t s);
str_t str_trim_end_ascii(str_t s);
str_t str_trim_start_ascii(str_t s);

/* Splitting */

str_split_scalar_t str_split_scalar(str_t s, u8 delimiter);

bool str_split_scalar_next(str_split_scalar_t *it, str_t *out);
