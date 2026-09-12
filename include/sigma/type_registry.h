#pragma once

/*
 * Define this before including sigma/printf.h to register borrowed custom
 * formatters:
 *
 *   #define SIGMA_CUSTOM_FORMAT_TYPES(X) \
 *     X(POINT, point_t, sigma_point_formatter)
 */
#ifndef SIGMA_CUSTOM_FORMAT_TYPES
#define SIGMA_CUSTOM_FORMAT_TYPES(X)
#endif

/* Maps character value types to the prefix of their concrete operations. */
#define SIGMA_CHARACTER_TYPES(X)                                               \
  X(char, sigma_char)                                                          \
  X(signed char, sigma_signed_char)                                            \
  X(unsigned char, ascii)                                                      \
  X(sigma_rune, sigma_rune)

/* Maps owning value types to their concrete clone and deinit operations. */
#define SIGMA_OWNED_TYPES(X)                                                   \
  X(STRING, string_t, string_clone, string_deinit)                             \
  X(STRING_VEC, string_vec, string_vec_clone, string_vec_deinit)

/*
 * Maps directly formattable value types to sigma_fmt_arg constructors.
 *
 * Formatting borrows values. Membership here says nothing about ownership or
 * destruction semantics.
 */
#define SIGMA_FORMAT_TYPES(X)                                                  \
  X(BOOL, bool, sigma_fmt_arg_bool)                                            \
  X(CHAR, char, sigma_fmt_arg_char)                                            \
  X(I8, signed char, sigma_fmt_arg_i64)                                        \
  X(U8, unsigned char, sigma_fmt_arg_u64)                                      \
  X(SHORT, short, sigma_fmt_arg_i64)                                           \
  X(USHORT, unsigned short, sigma_fmt_arg_u64)                                 \
  X(INT, int, sigma_fmt_arg_i64)                                               \
  X(UINT, unsigned int, sigma_fmt_arg_u64)                                     \
  X(LONG, long, sigma_fmt_arg_i64)                                             \
  X(ULONG, unsigned long, sigma_fmt_arg_u64)                                   \
  X(LLONG, long long, sigma_fmt_arg_i64)                                       \
  X(ULLONG, unsigned long long, sigma_fmt_arg_u64)                             \
  X(F32, float, sigma_fmt_arg_f32)                                             \
  X(F64, double, sigma_fmt_arg_f64)                                            \
  X(PTR, void *, sigma_fmt_arg_ptr)                                            \
  X(CONST_PTR, const void *, sigma_fmt_arg_ptr)                                \
  X(STR, str_t, sigma_fmt_arg_str)                                             \
  X(BYTES, bytes_t, sigma_fmt_arg_bytes)                                       \
  X(RUNE, sigma_rune, sigma_fmt_arg_rune)
