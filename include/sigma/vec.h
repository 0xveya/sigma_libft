#pragma once

#include <allocator_vtable.h>
#include <sigma/qol.h>

#include <stdbool.h>
#include <stdint.h>

/*
 * Defines a vector for trivial or borrowed elements.
 *
 * Generated init creates an empty vector. reserve grows capacity without
 * changing length. push and pop copy elements. get returns NULL out of bounds.
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
    Name result = *src;                                                        \
    *src = (Name){0};                                                          \
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
  static inline T *Name##_get(Name *vec, usize index) {                        \
    return index < vec->len ? &vec->items[index] : NULL;                       \
  }                                                                            \
                                                                               \
  static inline const T *Name##_get_const(const Name *vec, usize index) {      \
    return index < vec->len ? &vec->items[index] : NULL;                       \
  }                                                                            \
                                                                               \
  static inline bool Name##_reserve(Name *vec, usize new_cap) {                \
    if (new_cap <= vec->cap)                                                   \
      return true;                                                             \
                                                                               \
    if (new_cap > SIZE_MAX / sizeof(T))                                        \
      return false;                                                            \
                                                                               \
    T *items = (T *)allocator_realloc_aligned_debug(                           \
        vec->allocator, vec->items, vec->cap * sizeof(T), new_cap * sizeof(T), \
        _Alignof(T), __FILE__, __func__, __LINE__);                            \
                                                                               \
    if (items == NULL)                                                         \
      return false;                                                            \
                                                                               \
    vec->items = items;                                                        \
    vec->cap = new_cap;                                                        \
    return true;                                                               \
  }                                                                            \
                                                                               \
  static inline bool Name##_push(Name *vec, T value) {                         \
    if (vec->len == vec->cap) {                                                \
      usize new_cap = 8;                                                       \
                                                                               \
      if (vec->cap != 0) {                                                     \
        if (vec->cap > SIZE_MAX / 2)                                           \
          return false;                                                        \
        new_cap = vec->cap * 2;                                                \
      }                                                                        \
                                                                               \
      if (!Name##_reserve(vec, new_cap))                                       \
        return false;                                                          \
    }                                                                          \
                                                                               \
    vec->items[vec->len++] = value;                                            \
    return true;                                                               \
  }                                                                            \
                                                                               \
  static inline bool Name##_pop(Name *vec, T *out) {                           \
    if (vec->len == 0)                                                         \
      return false;                                                            \
                                                                               \
    --vec->len;                                                                \
    if (out != NULL)                                                           \
      *out = vec->items[vec->len];                                             \
                                                                               \
    return true;                                                               \
  }                                                                            \
                                                                               \
  static inline void Name##_clear(Name *vec) { vec->len = 0; }

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
#define SIGMA_VEC_DEFINE_OWNED(T, Name, deinit_fn)                             \
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
  static inline bool Name##_reserve(Name *vec, usize new_cap) {                \
    if (new_cap <= vec->cap)                                                   \
      return true;                                                             \
                                                                               \
    if (new_cap > SIZE_MAX / sizeof(T))                                        \
      return false;                                                            \
                                                                               \
    T *items = (T *)allocator_realloc_aligned_debug(                           \
        vec->allocator, vec->items, vec->cap * sizeof(T), new_cap * sizeof(T), \
        _Alignof(T), __FILE__, __func__, __LINE__);                            \
                                                                               \
    if (items == NULL)                                                         \
      return false;                                                            \
                                                                               \
    vec->items = items;                                                        \
    vec->cap = new_cap;                                                        \
    return true;                                                               \
  }                                                                            \
                                                                               \
  static inline bool Name##_ensure_push(Name *vec) {                           \
    if (vec->len < vec->cap)                                                   \
      return true;                                                             \
                                                                               \
    usize new_cap = 8;                                                         \
                                                                               \
    if (vec->cap != 0) {                                                       \
      if (vec->cap > SIZE_MAX / 2)                                             \
        return false;                                                          \
      new_cap = vec->cap * 2;                                                  \
    }                                                                          \
                                                                               \
    return Name##_reserve(vec, new_cap);                                       \
  }                                                                            \
                                                                               \
  static inline bool Name##_push_take(Name *vec, T *src) {                     \
    if (!Name##_ensure_push(vec))                                              \
      return false;                                                            \
                                                                               \
    vec->items[vec->len++] = *src;                                             \
    *src = (T){0};                                                             \
                                                                               \
    return true;                                                               \
  }                                                                            \
                                                                               \
  static inline bool Name##_pop_take(Name *vec, T *out) {                      \
    if (vec->len == 0 || out == NULL)                                          \
      return false;                                                            \
                                                                               \
    --vec->len;                                                                \
    *out = vec->items[vec->len];                                               \
    vec->items[vec->len] = (T){0};                                             \
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
  static inline Name Name##_take(Name *src) {                                  \
    Name result = *src;                                                        \
    *src = (Name){0};                                                          \
    return result;                                                             \
  }
