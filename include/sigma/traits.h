#pragma once

#include <sigma/ascii.h>
#include <sigma/diagnostic.h>
#include <sigma/meta.h>
#include <sigma/ownership.h>
#include <sigma/printf.h>
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

#define SIGMA_CHAR_SUPPORTED_ASSOC(type, prefix)                               \
  type:                                                                        \
  true,
#define SIGMA_CHAR_TYPE_SUPPORTED(value)                                       \
  _Generic((value),                                                            \
      SIGMA_CHARACTER_TYPES(SIGMA_CHAR_SUPPORTED_ASSOC) default: false)
#define SIGMA_CHAR_PREDICATE(associations, operation, value)                   \
  SIGMA_REQUIRE_EXPR(SIGMA_CHAR_TYPE_SUPPORTED(value),                         \
                     "SIGMA_E_CHARACTER_TYPE",                                 \
                     "unsupported character operation type", operation(value), \
                     "use char, signed char, unsigned char, or sigma_rune",    \
                     _Generic((value),                                         \
                         SIGMA_CHARACTER_TYPES(associations) default: (        \
                                  bool (*)(typeof(value)))0)(value))
#define SIGMA_CHAR_TRANSFORM(associations, operation, value)                   \
  SIGMA_REQUIRE_EXPR(SIGMA_CHAR_TYPE_SUPPORTED(value),                         \
                     "SIGMA_E_CHARACTER_TYPE",                                 \
                     "unsupported character operation type", operation(value), \
                     "use char, signed char, unsigned char, or sigma_rune",    \
                     _Generic((value),                                         \
                         SIGMA_CHARACTER_TYPES(associations) default: (        \
                                  typeof(value) (*)(typeof(value)))0)(value))

/* Dispatches character operations by value type. */
#define sigma_isalnum(value)                                                   \
  SIGMA_CHAR_PREDICATE(SIGMA_CHAR_ALNUM_ASSOC, sigma_isalnum, value)
#define sigma_isalpha(value)                                                   \
  SIGMA_CHAR_PREDICATE(SIGMA_CHAR_ALPHA_ASSOC, sigma_isalpha, value)
#define sigma_iscntrl(value)                                                   \
  SIGMA_CHAR_PREDICATE(SIGMA_CHAR_CNTRL_ASSOC, sigma_iscntrl, value)
#define sigma_isdigit(value)                                                   \
  SIGMA_CHAR_PREDICATE(SIGMA_CHAR_DIGIT_ASSOC, sigma_isdigit, value)
#define sigma_islower(value)                                                   \
  SIGMA_CHAR_PREDICATE(SIGMA_CHAR_LOWER_ASSOC, sigma_islower, value)
#define sigma_isprint(value)                                                   \
  SIGMA_CHAR_PREDICATE(SIGMA_CHAR_PRINT_ASSOC, sigma_isprint, value)
#define sigma_isspace(value)                                                   \
  SIGMA_CHAR_PREDICATE(SIGMA_CHAR_SPACE_ASSOC, sigma_isspace, value)
#define sigma_isupper(value)                                                   \
  SIGMA_CHAR_PREDICATE(SIGMA_CHAR_UPPER_ASSOC, sigma_isupper, value)
#define sigma_isxdigit(value)                                                  \
  SIGMA_CHAR_PREDICATE(SIGMA_CHAR_XDIGIT_ASSOC, sigma_isxdigit, value)
#define sigma_tolower(value)                                                   \
  SIGMA_CHAR_TRANSFORM(SIGMA_CHAR_TO_LOWER_ASSOC, sigma_tolower, value)
#define sigma_toupper(value)                                                   \
  SIGMA_CHAR_TRANSFORM(SIGMA_CHAR_TO_UPPER_ASSOC, sigma_toupper, value)

#define SIGMA_CLONE_ASSOC(tag, type, clone, deinit)                            \
  type * : clone, const type * : clone,
#define SIGMA_DEINIT_ASSOC(tag, type, clone, deinit) type * : deinit,
#define SIGMA_OWNED_CONST_PTR_SUPPORTED_ASSOC(tag, type, clone, deinit)        \
  type * : true, const type * : true,
#define SIGMA_OWNED_PTR_SUPPORTED_ASSOC(tag, type, clone, deinit) type * : true,
#define SIGMA_CLONE_TYPE_SUPPORTED(value)                                      \
  _Generic(&(value),                                                           \
      SIGMA_OWNED_TYPES(SIGMA_OWNED_CONST_PTR_SUPPORTED_ASSOC) default: false)
#define SIGMA_DEINIT_TYPE_SUPPORTED(value)                                     \
  _Generic(&(value),                                                           \
      SIGMA_OWNED_TYPES(SIGMA_OWNED_PTR_SUPPORTED_ASSOC) default: false)

/* Clones src into an empty out value without changing either on failure. */
#define sigma_clone(out, src)                                                  \
  SIGMA_REQUIRE_EXPR(                                                          \
      SIGMA_CLONE_TYPE_SUPPORTED(src), "SIGMA_E_CLONE_TYPE",                   \
      "unsupported owning type for sigma_clone", src,                          \
      "register the owning type in SIGMA_OWNED_TYPES",                         \
      SIGMA_REQUIRE_EXPR(                                                      \
          SIGMA_SAME_TYPE((out), (src)), "SIGMA_E_CLONE_OUTPUT_TYPE",          \
          "clone output and source have different types", out = clone(src),    \
          "make out and src the same owning type",                             \
          _Generic(&(src),                                                     \
              SIGMA_OWNED_TYPES(SIGMA_CLONE_ASSOC) default: (bool (*)(         \
                  typeof(&(out)), typeof(&(src))))0)(&(out), &(src))))

/* Destroys an owning value through its registered concrete deinitializer. */
#define sigma_deinit(value)                                                    \
  SIGMA_REQUIRE_EXPR(SIGMA_DEINIT_TYPE_SUPPORTED(value),                       \
                     "SIGMA_E_DEINIT_TYPE",                                    \
                     "unsupported owning type for sigma_deinit", value,        \
                     "register the owning type in SIGMA_OWNED_TYPES",          \
                     _Generic(&(value),                                        \
                         SIGMA_OWNED_TYPES(SIGMA_DEINIT_ASSOC) default: (      \
                             void (*)(typeof(&(value))))0)(&(value)))

/* Destroys dst, transfers src into it, and leaves src moved from. */
#define sigma_replace(dst, src)                                                \
  do {                                                                         \
    SIGMA_STATIC_ASSERT(SIGMA_SAME_TYPE((dst), (src)), "SIGMA_E_REPLACE_TYPE", \
                        "replacement values have different types", dst = src,  \
                        "make dst and src the same owning type");              \
    sigma_deinit(dst);                                                         \
    SIGMA_MOVE(dst, src);                                                      \
  } while (0)
