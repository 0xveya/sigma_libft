#pragma once

/*
 * Moves src into an uninitialized or empty dst, then resets src to {0}.
 *
 * Both arguments must be assignable lvalues of compatible types. dst must not
 * own a live resource because its previous value is overwritten without being
 * deinitialized. Prefer a type's take function in ordinary API code.
 */
#define SIGMA_TAKE(dst, src)                                                   \
  do {                                                                         \
    (dst) = (src);                                                             \
    (src) = (typeof(src)){0};                                                  \
  } while (0)
