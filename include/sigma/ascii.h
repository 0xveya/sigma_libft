#pragma once

#include <sigma/qol.h>

#define SIGMA_ASCII_FLAGS(X)                                                   \
  X(CONTROL, 1u << 0)                                                          \
  X(SPACE, 1u << 1)                                                            \
  X(DIGIT, 1u << 2)                                                            \
  X(UPPER, 1u << 3)                                                            \
  X(LOWER, 1u << 4)                                                            \
  X(HEX, 1u << 5)                                                              \
  X(PUNCT, 1u << 6)                                                            \
  X(PRINT, 1u << 7)

#define SIGMA_ASCII_ENUM(name, value) SIGMA_ASCII_##name = value,

enum sigma_ascii_flag : u8 { SIGMA_ASCII_FLAGS(SIGMA_ASCII_ENUM) };

_Static_assert(sizeof(enum sigma_ascii_flag) == sizeof(u8),
               "ASCII flags must fit in one byte");

#undef SIGMA_ASCII_ENUM
#undef SIGMA_ASCII_FLAGS

typedef struct {
  u8 flags;
  u8 lower;
  u8 upper;
} sigma_ascii_info_t;

/* Returns true when c is an ASCII letter or decimal digit. */
bool ascii_is_alnum(u8 c);
/* Returns true when c is an ASCII letter. */
bool ascii_is_alpha(u8 c);
/* Returns true when c is an ASCII decimal digit. */
bool ascii_is_digit(u8 c);
/* Returns true when c is an ASCII control character. */
bool ascii_is_cntrl(u8 c);
/* Returns true when c is a lowercase ASCII letter. */
bool ascii_is_lower(u8 c);
/* Returns true when c is a printable ASCII character. */
bool ascii_is_print(u8 c);
/* Returns true when c is ASCII whitespace. */
bool ascii_is_space(u8 c);
/* Returns true when c is an uppercase ASCII letter. */
bool ascii_is_upper(u8 c);
/* Returns true when c is an ASCII hexadecimal digit. */
bool ascii_is_xdigit(u8 c);

/* Converts an ASCII letter to lowercase and leaves other bytes unchanged. */
u8 ascii_to_lower(u8 c);
/* Converts an ASCII letter to uppercase and leaves other bytes unchanged. */
u8 ascii_to_upper(u8 c);
