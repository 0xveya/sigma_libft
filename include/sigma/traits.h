#pragma once

#include <sigma/ascii.h>
#include <sigma/format.h>
#include <sigma/meta.h>
#include <sigma/ownership.h>
#include <sigma/string_vec.h>
#include <sigma/type_registry.h>
#include <sigma/unicode.h>

#define SIGMA_CHAR_ALNUM_ASSOC(type, prefix)                                   \
  type:                                                                        \
  SIGMA_CAT(prefix, _is_alnum),
#define SIGMA_CHAR_ALPHA_ASSOC(type, prefix)                                   \
  type:                                                                        \
  SIGMA_CAT(prefix, _is_alpha),
#define SIGMA_CHAR_CNTRL_ASSOC(type, prefix)                                   \
  type:                                                                        \
  SIGMA_CAT(prefix, _is_cntrl),
#define SIGMA_CHAR_DIGIT_ASSOC(type, prefix)                                   \
  type:                                                                        \
  SIGMA_CAT(prefix, _is_digit),
#define SIGMA_CHAR_LOWER_ASSOC(type, prefix)                                   \
  type:                                                                        \
  SIGMA_CAT(prefix, _is_lower),
#define SIGMA_CHAR_PRINT_ASSOC(type, prefix)                                   \
  type:                                                                        \
  SIGMA_CAT(prefix, _is_print),
#define SIGMA_CHAR_SPACE_ASSOC(type, prefix)                                   \
  type:                                                                        \
  SIGMA_CAT(prefix, _is_space),
#define SIGMA_CHAR_UPPER_ASSOC(type, prefix)                                   \
  type:                                                                        \
  SIGMA_CAT(prefix, _is_upper),
#define SIGMA_CHAR_XDIGIT_ASSOC(type, prefix)                                  \
  type:                                                                        \
  SIGMA_CAT(prefix, _is_xdigit),
#define SIGMA_CHAR_TO_LOWER_ASSOC(type, prefix)                                \
  type:                                                                        \
  SIGMA_CAT(prefix, _to_lower),
#define SIGMA_CHAR_TO_UPPER_ASSOC(type, prefix)                                \
  type:                                                                        \
  SIGMA_CAT(prefix, _to_upper),

/* Dispatches character operations by value type. */
#define sigma_isalnum(value)                                                   \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_ALNUM_ASSOC) default: 0)(value)
#define sigma_isalpha(value)                                                   \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_ALPHA_ASSOC) default: 0)(value)
#define sigma_iscntrl(value)                                                   \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_CNTRL_ASSOC) default: 0)(value)
#define sigma_isdigit(value)                                                   \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_DIGIT_ASSOC) default: 0)(value)
#define sigma_islower(value)                                                   \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_LOWER_ASSOC) default: 0)(value)
#define sigma_isprint(value)                                                   \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_PRINT_ASSOC) default: 0)(value)
#define sigma_isspace(value)                                                   \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_SPACE_ASSOC) default: 0)(value)
#define sigma_isupper(value)                                                   \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_UPPER_ASSOC) default: 0)(value)
#define sigma_isxdigit(value)                                                  \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_XDIGIT_ASSOC) default: 0)(value)
#define sigma_tolower(value)                                                   \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_TO_LOWER_ASSOC) default: 0)(value)
#define sigma_toupper(value)                                                   \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_TO_UPPER_ASSOC) default: 0)(value)

#define SIGMA_CLONE_ASSOC(tag, type, clone, deinit)                            \
  type * : clone, const type * : clone,
#define SIGMA_DEINIT_ASSOC(tag, type, clone, deinit) type * : deinit,
#define SIGMA_FORMAT_ASSOC(type, constructor)                                  \
  type:                                                                        \
  constructor,

/* Converts a concrete value into a borrowed type-erased formatting argument. */
#define sigma_fmt_arg(value)                                                   \
  _Generic((value), SIGMA_FORMAT_TYPES(SIGMA_FORMAT_ASSOC) default: 0)(value)

/* Clones src into an empty out value without changing either on failure. */
#define sigma_clone(out, src)                                                  \
  _Generic(&(src), SIGMA_OWNED_TYPES(SIGMA_CLONE_ASSOC) default: 0)(&(out),    \
                                                                    &(src))

/* Destroys an owning value through its registered concrete deinitializer. */
#define sigma_deinit(value)                                                    \
  _Generic(&(value), SIGMA_OWNED_TYPES(SIGMA_DEINIT_ASSOC) default: 0)(&(value))

/* Destroys dst, transfers src into it, and leaves src moved from. */
#define sigma_replace(dst, src)                                                \
  do {                                                                         \
    _Static_assert(SIGMA_SAME_TYPE((dst), (src)),                              \
                   "sigma_replace: type mismatch");                            \
    sigma_deinit(dst);                                                         \
    SIGMA_MOVE(dst, src);                                                      \
  } while (0)
