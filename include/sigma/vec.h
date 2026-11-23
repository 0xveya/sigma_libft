#pragma once

#include <allocator_vtable.h>
#include <sigma/mem.h>
#include <sigma/ownership.h>
#include <sigma/qol.h>

#include <stdbool.h>
#include <stdint.h>

/*
 * Defines a vector for trivial or borrowed elements.
 *
 * Generated init creates an empty vector. reserve grows capacity without
 * changing length. push and pop copy elements. get and pop return optionals.
 * clear and deinit never deinitialize individual elements. take transfers the
 * backing allocation and resets its source to {0}. Views borrow the vector
 * storage and remain valid only until growth, take, or deinit.
 */
#define SIGMA_VEC_DEFINE(T, Name)                                              \
  typedef struct {                                                             \
    T *items;                                                                  \
    usize len;                                                                 \
    usize cap;                                                                 \
    allocator_t allocator;                                                     \
  } Name;                                                                      \
                                                                               \
  typedef struct {                                                             \
    const T *items;                                                            \
    usize len;                                                                 \
  } Name##_view_t;                                                             \
                                                                               \
  typedef struct {                                                             \
    T *items;                                                                  \
    usize len;                                                                 \
  } Name##_view_mut_t;                                                         \
                                                                               \
  static inline Name Name##_init(allocator_t allocator) {                      \
    return (Name){.allocator = allocator};                                     \
  }                                                                            \
                                                                               \
  static inline void Name##_deinit(Name *vec) {                                \
    if (vec->items != NULL)                                                    \
      allocator_free_aligned(vec->allocator, vec->items, vec->cap * sizeof(T), \
                             _Alignof(T));                                     \
    *vec = (Name){0};                                                          \
  }                                                                            \
                                                                               \
  static inline Name Name##_take(Name *src) {                                  \
    Name result = {0};                                                         \
    SIGMA_MOVE_PTR(&result, src);                                              \
    return result;                                                             \
  }                                                                            \
                                                                               \
  static inline Name##_view_t Name##_view(const Name *vec) {                   \
    return (Name##_view_t){                                                    \
        .items = vec->items,                                                   \
        .len = vec->len,                                                       \
    };                                                                         \
  }                                                                            \
                                                                               \
  static inline Name##_view_mut_t Name##_view_mut(Name *vec) {                 \
    return (Name##_view_mut_t){                                                \
        .items = vec->items,                                                   \
        .len = vec->len,                                                       \
    };                                                                         \
  }                                                                            \
                                                                               \
  static inline bool Name##_is_empty(const Name *vec) {                        \
    return vec->len == 0;                                                      \
  }                                                                            \
                                                                               \
  OPTIONAL(Name##_ref, T *);                                                   \
  OPTIONAL(Name##_const_ref, const T *);                                       \
  OPTIONAL(Name##_optional, T);                                                \
                                                                               \
  static inline Name##_ref Name##_get(Name *vec, usize index) {                \
    return index < vec->len ? SOME(Name##_ref, &vec->items[index])             \
                            : NONE(Name##_ref);                                \
  }                                                                            \
                                                                               \
  static inline Name##_const_ref Name##_get_const(const Name *vec,             \
                                                  usize index) {               \
    return index < vec->len ? SOME(Name##_const_ref, &vec->items[index])       \
                            : NONE(Name##_const_ref);                          \
  }                                                                            \
  static inline sigma_alloc_status Name##_reserve(Name *vec, usize new_cap) {  \
    if (new_cap <= vec->cap)                                                   \
      return OK(sigma_alloc_status, sigma_unit);                               \
                                                                               \
    if (new_cap > SIZE_MAX / sizeof(T))                                        \
      return ERR(sigma_alloc_status, sigma_alloc_size_overflow);               \
                                                                               \
    T *items = (T *)allocator_realloc_aligned_debug(                           \
        vec->allocator, vec->items, vec->cap * sizeof(T), new_cap * sizeof(T), \
        _Alignof(T), __FILE__, __func__, __LINE__);                            \
                                                                               \
    if (items == NULL)                                                         \
      return ERR(sigma_alloc_status, sigma_alloc_out_of_memory);               \
                                                                               \
    vec->items = items;                                                        \
    vec->cap = new_cap;                                                        \
    return OK(sigma_alloc_status, sigma_unit);                                 \
  }                                                                            \
                                                                               \
  static inline sigma_alloc_status Name##_push(Name *vec, T value) {           \
    if (vec->len == vec->cap) {                                                \
      usize new_cap = 8;                                                       \
                                                                               \
      if (vec->cap != 0) {                                                     \
        if (vec->cap > SIZE_MAX / 2)                                           \
          return ERR(sigma_alloc_status, sigma_alloc_size_overflow);           \
        new_cap = vec->cap * 2;                                                \
      }                                                                        \
                                                                               \
      (void)TRY(sigma_alloc_status, Name##_reserve(vec, new_cap));             \
    }                                                                          \
                                                                               \
    vec->items[vec->len++] = value;                                            \
    return OK(sigma_alloc_status, sigma_unit);                                 \
  }                                                                            \
                                                                               \
  static inline Name##_optional Name##_pop(Name *vec) {                        \
    if (vec->len == 0)                                                         \
      return NONE(Name##_optional);                                            \
    return SOME(Name##_optional, vec->items[--vec->len]);                      \
  }                                                                            \
  static inline void Name##_clear(Name *vec) { vec->len = 0; }

/* Documents that generated elements are trivial values. */
#define SIGMA_VEC_DEFINE_TRIVIAL(T, Name) SIGMA_VEC_DEFINE(T, Name)

/* Documents that generated elements borrow resources owned elsewhere. */
#define SIGMA_VEC_DEFINE_BORROWED(T, Name) SIGMA_VEC_DEFINE(T, Name)

/*
 * Defines a vector that owns every inserted element.
 *
 * Generated init creates an empty vector. reserve and ensure_push prepare
 * capacity without consuming an element. deinit_fn must accept T * and leave
 * the element safely deinitialized. push_take consumes src only after growth
 * succeeds, leaving vec and src unchanged on failure. pop_take transfers the
 * final element into an uninitialized or empty out value and clears its slot.
 * clear and deinit destroy every remaining element. take transfers the whole
 * vector and resets its source to {0}.
 */
#define SIGMA_VEC_DEFINE_OWNED(T, Name, clone_fn, deinit_fn)                   \
  typedef struct {                                                             \
    T *items;                                                                  \
    usize len;                                                                 \
    usize cap;                                                                 \
    allocator_t allocator;                                                     \
  } Name;                                                                      \
                                                                               \
  static inline Name Name##_init(allocator_t allocator) {                      \
    return (Name){.allocator = allocator};                                     \
  }                                                                            \
                                                                               \
  static inline sigma_alloc_status Name##_reserve(Name *vec, usize new_cap) {  \
    if (new_cap <= vec->cap)                                                   \
      return OK(sigma_alloc_status, sigma_unit);                               \
                                                                               \
    if (new_cap > SIZE_MAX / sizeof(T))                                        \
      return ERR(sigma_alloc_status, sigma_alloc_size_overflow);               \
                                                                               \
    T *items = (T *)allocator_realloc_aligned_debug(                           \
        vec->allocator, vec->items, vec->cap * sizeof(T), new_cap * sizeof(T), \
        _Alignof(T), __FILE__, __func__, __LINE__);                            \
                                                                               \
    if (items == NULL)                                                         \
      return ERR(sigma_alloc_status, sigma_alloc_out_of_memory);               \
                                                                               \
    vec->items = items;                                                        \
    vec->cap = new_cap;                                                        \
    return OK(sigma_alloc_status, sigma_unit);                                 \
  }                                                                            \
                                                                               \
  static inline sigma_alloc_status Name##_ensure_push(Name *vec) {             \
    if (vec->len < vec->cap)                                                   \
      return OK(sigma_alloc_status, sigma_unit);                               \
                                                                               \
    usize new_cap = 8;                                                         \
                                                                               \
    if (vec->cap != 0) {                                                       \
      if (vec->cap > SIZE_MAX / 2)                                             \
        return ERR(sigma_alloc_status, sigma_alloc_size_overflow);             \
      new_cap = vec->cap * 2;                                                  \
    }                                                                          \
                                                                               \
    return Name##_reserve(vec, new_cap);                                       \
  }                                                                            \
                                                                               \
  static inline sigma_alloc_status Name##_push_take(Name *vec, T *src) {       \
    (void)TRY(sigma_alloc_status, Name##_ensure_push(vec));                    \
                                                                               \
    SIGMA_MOVE_PTR(&vec->items[vec->len], src);                                \
    ++vec->len;                                                                \
                                                                               \
    return OK(sigma_alloc_status, sigma_unit);                                 \
  }                                                                            \
                                                                               \
  static inline bool Name##_pop_take(Name *vec, T *out) {                      \
    if (vec->len == 0 || out == NULL)                                          \
      return false;                                                            \
                                                                               \
    --vec->len;                                                                \
    SIGMA_MOVE_PTR(out, &vec->items[vec->len]);                                \
                                                                               \
    return true;                                                               \
  }                                                                            \
                                                                               \
  static inline void Name##_clear(Name *vec) {                                 \
    for (usize i = 0; i < vec->len; ++i)                                       \
      deinit_fn(&vec->items[i]);                                               \
                                                                               \
    vec->len = 0;                                                              \
  }                                                                            \
                                                                               \
  static inline void Name##_deinit(Name *vec) {                                \
    Name##_clear(vec);                                                         \
                                                                               \
    if (vec->items != NULL)                                                    \
      allocator_free_aligned(vec->allocator, vec->items, vec->cap * sizeof(T), \
                             _Alignof(T));                                     \
                                                                               \
    *vec = (Name){0};                                                          \
  }                                                                            \
                                                                               \
  static inline bool Name##_clone(Name *out, const Name *src) {                \
    Name result = Name##_init(src->allocator);                                 \
                                                                               \
    if (!Name##_reserve(&result, src->len).ok)                                 \
      return false;                                                            \
                                                                               \
    for (usize i = 0; i < src->len; ++i) {                                     \
      if (!clone_fn(&result.items[i], &src->items[i])) {                       \
        Name##_deinit(&result);                                                \
        return false;                                                          \
      }                                                                        \
      ++result.len;                                                            \
    }                                                                          \
                                                                               \
    *out = result;                                                             \
    return true;                                                               \
  }                                                                            \
                                                                               \
  static inline Name Name##_take(Name *src) {                                  \
    Name result = {0};                                                         \
    SIGMA_MOVE_PTR(&result, src);                                              \
    return result;                                                             \
  }
