#pragma once

#include <allocator_vtable.h>
#include <sigma/attributes.h>
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

str_split_scalar_t str_split_scalar(str_t s, u8 delimiter);

bool str_split_scalar_next(str_split_scalar_t *it, str_t *out);

#define STR_LIT(s) ((str_t){.items = (s), .len = sizeof(s) - 1})

#define STR_NPOS ((usize) - 1)

str_t str_from_cstr(const char *s);
SIGMA_NODISCARD char *str_to_cstr(str_t s, allocator_t allocator);

bool str_is_empty(str_t s);

str_t str_sub(str_t s, usize start, usize len);

i32 str_cmp(str_t a, str_t b);
bool str_eq(str_t a, str_t b);

bool str_ends_with(str_t s, str_t suffix);
bool str_starts_with(str_t s, str_t prefix);

usize str_find_byte(str_t s, u8 byte);
usize str_rfind_byte(str_t s, u8 byte);

str_t str_trim_ascii(str_t s);
str_t str_trim_end_ascii(str_t s);
str_t str_trim_start_ascii(str_t s);
