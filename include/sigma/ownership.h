#pragma once

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
    static_assert(SIGMA_SAME_TYPE((dst), (src)), "SIGMA_MOVE: type mismatch"); \
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
    static_assert(SIGMA_SAME_TYPE((dst), (src)), "SIGMA_TAKE: type mismatch"); \
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
    static_assert(SIGMA_SAME_TYPE(*(dst), *(src)),                             \
                  "SIGMA_MOVE_PTR: type mismatch");                            \
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
    static_assert(SIGMA_SAME_TYPE((a), (b)), "SIGMA_SWAP: type mismatch");     \
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
