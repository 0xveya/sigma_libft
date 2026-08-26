#pragma once

/* Maps character value types to the prefix of their concrete operations. */
#define SIGMA_CHARACTER_TYPES(X)                                               \
  X(char, ascii)                                                               \
  X(signed char, ascii)                                                        \
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
  X(bool, sigma_fmt_arg_bool)                                                  \
  X(char, sigma_fmt_arg_char)                                                  \
  X(signed char, sigma_fmt_arg_i64)                                            \
  X(unsigned char, sigma_fmt_arg_u64)                                          \
  X(short, sigma_fmt_arg_i64)                                                  \
  X(unsigned short, sigma_fmt_arg_u64)                                         \
  X(int, sigma_fmt_arg_i64)                                                    \
  X(unsigned int, sigma_fmt_arg_u64)                                           \
  X(long, sigma_fmt_arg_i64)                                                   \
  X(unsigned long, sigma_fmt_arg_u64)                                          \
  X(long long, sigma_fmt_arg_i64)                                              \
  X(unsigned long long, sigma_fmt_arg_u64)                                     \
  X(float, sigma_fmt_arg_f32)                                                  \
  X(double, sigma_fmt_arg_f64)                                                 \
  X(str_t, sigma_fmt_arg_str)                                                  \
  X(bytes_t, sigma_fmt_arg_bytes)                                              \
  X(sigma_rune, sigma_fmt_arg_rune)
