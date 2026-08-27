#pragma once

#include <sigma/diagnostic.h>

/*
 * Compile-time type compatibility check.
 *
 * This provides type safety for Sigma ownership helpers, but does not make
 * owning structs non-copyable. Ordinary C struct assignment remains legal.
 */
#define SIGMA_SAME_TYPE(a, b) _Generic((a), typeof(b): true, default: false)

/*
 * Set value to its canonical zero state.
 *
 * This does NOT destroy resources owned by value.
 * Using this on a live owner without first transferring or destroying its
 * resource will leak that resource.
 */
#define SIGMA_ZERO(value)                                                      \
  do {                                                                         \
    (value) = (typeof(value)){0};                                              \
  } while (0)

/*
 * Transfer src into dst and leave src in its canonical zero state.
 *
 * dst must not currently own a live resource.
 *
 * After this operation:
 *   - dst contains src's previous value.
 *   - src is zeroed and considered moved-from.
 *
 * No allocation or destruction occurs.
 */
#define SIGMA_MOVE(dst, src)                                                   \
  do {                                                                         \
    SIGMA_STATIC_ASSERT(                                                       \
        SIGMA_SAME_TYPE((dst), (src)), "SIGMA_E_MOVE_TYPE",                    \
        "cannot move between different types", dst = src,                      \
        "make dst and src the same type or convert src explicitly");           \
    (dst) = (src);                                                             \
    (src) = (typeof(src)){0};                                                  \
  } while (0)

/*
 * Transfer src into dst using "take" terminology.
 *
 * This has the same ownership semantics and type checking as SIGMA_MOVE.
 */
#define SIGMA_TAKE(dst, src)                                                   \
  do {                                                                         \
    SIGMA_STATIC_ASSERT(                                                       \
        SIGMA_SAME_TYPE((dst), (src)), "SIGMA_E_TAKE_TYPE",                    \
        "cannot take between different types", dst = src,                      \
        "make dst and src the same type or convert src explicitly");           \
    (dst) = (src);                                                             \
    (src) = (typeof(src)){0};                                                  \
  } while (0)

/*
 * Transfer *src into *dst and leave *src in its canonical zero state.
 *
 * *dst must not currently own a live resource.
 *
 * This is the pointer-oriented equivalent of SIGMA_MOVE.
 */
#define SIGMA_MOVE_PTR(dst, src)                                               \
  do {                                                                         \
    SIGMA_STATIC_ASSERT(                                                       \
        SIGMA_SAME_TYPE(*(dst), *(src)), "SIGMA_E_MOVE_PTR_TYPE",              \
        "cannot move through pointers to different types", *dst = *src,        \
        "pass pointers to the same type or convert the value explicitly");     \
    *(dst) = *(src);                                                           \
    *(src) = (typeof(*(src))){0};                                              \
  } while (0)

/*
 * Exchange two values.
 *
 * For owning values, ownership follows the exchanged values.
 * No allocation, cloning, or destruction occurs.
 */
#define SIGMA_SWAP(a, b)                                                       \
  do {                                                                         \
    SIGMA_STATIC_ASSERT(                                                       \
        SIGMA_SAME_TYPE((a), (b)), "SIGMA_E_SWAP_TYPE",                        \
        "cannot swap values of different types", a <->b,                       \
        "make both values the same type before swapping them");                \
    typeof(a) sigma_swap_tmp__ = (a);                                          \
    (a) = (b);                                                                 \
    (b) = sigma_swap_tmp__;                                                    \
  } while (0)

/*
 * Abandon value without destroying its resource and leave it zeroed.
 *
 * This deliberately bypasses normal destruction. For ordinary heap-owning
 * values this leaks the resource unless ownership has been transferred to
 * something outside Sigma's ownership model.
 */
#define SIGMA_FORGET(value)                                                    \
  do {                                                                         \
    (value) = (typeof(value)){0};                                              \
  } while (0)
