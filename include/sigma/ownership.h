#pragma once

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
    (dst) = (src);                                                             \
    (src) = (typeof(src)){0};                                                  \
  } while (0)

/*
 * Alias for SIGMA_MOVE for APIs where "take" better describes the ownership
 * transfer.
 */
#define SIGMA_TAKE(dst, src) SIGMA_MOVE((dst), (src))

/*
 * Transfer *src into *dst and leave *src in its canonical zero state.
 *
 * *dst must not currently own a live resource.
 *
 * This is the pointer-oriented equivalent of SIGMA_MOVE.
 */
#define SIGMA_MOVE_PTR(dst, src)                                               \
  do {                                                                         \
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
