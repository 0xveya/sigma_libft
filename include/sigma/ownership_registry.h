#pragma once

#include <sigma/ownership.h>
#include <sigma/string.h>
#include <sigma/string_vec.h>

/*
 * Resolve an owning Sigma type to its destructor at compile time.
 *
 * Every type registered here must:
 *   - have a valid zero/moved-from state;
 *   - provide a deinit function accepting T *;
 *   - leave the value in its zero state after deinitialization.
 *
 * Add new owning resource types to this registry as they are introduced.
 */
#define SIGMA_DEINIT_FN(value)                                                 \
  _Generic(&(value), string_t *: string_deinit, string_vec *: string_vec_deinit)

/*
 * Destroy the resource owned by value.
 *
 * The concrete deinit function is selected at compile time through
 * SIGMA_DEINIT_FN.
 *
 * After destruction, value must be in its canonical zero state.
 */
#define SIGMA_DROP(value)                                                      \
  do {                                                                         \
    SIGMA_DEINIT_FN(value)(&(value));                                          \
  } while (0)

/*
 * Destroy dst's current resource and transfer src into dst.
 *
 * Unlike SIGMA_MOVE, dst may already own a live resource.
 *
 * After this operation:
 *   - dst's previous resource has been destroyed;
 *   - dst owns src's previous resource;
 *   - src is in its canonical zero/moved-from state.
 */
#define SIGMA_REPLACE(dst, src)                                                \
  do {                                                                         \
    SIGMA_DROP(dst);                                                           \
    SIGMA_MOVE(dst, src);                                                      \
  } while (0)
