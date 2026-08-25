#pragma once

/* Maps character value types to the prefix of their concrete operations. */
#define SIGMA_CHARACTER_TYPES(X)                                               \
  X(char, ascii)                                                               \
  X(signed char, ascii)                                                        \
  X(unsigned char, ascii)

/* Maps owning value types to their concrete clone and deinit operations. */
#define SIGMA_OWNED_TYPES(X)                                                   \
  X(STRING, string_t, string_clone, string_deinit)                             \
  X(STRING_VEC, string_vec, string_vec_clone, string_vec_deinit)
