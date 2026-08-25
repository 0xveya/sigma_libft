#pragma once

#include <allocator_vtable.h>
#include <sigma/qol.h>
#include <sigma/str.h>

#include <stdbool.h>

/* Owns a growable byte string and the allocation that backs it. */
typedef struct {
  char *items;
  usize len;
  usize cap;
  allocator_t allocator;
} string_t;

/* Lifetime */

/* Creates an empty string that will allocate through allocator. */
string_t string_init(allocator_t allocator);

/* Frees the backing allocation and resets string to {0}. */
void string_deinit(string_t *string);

/* Transfers ownership from source and resets source to {0}. */
string_t string_take(string_t *source);

/* Borrow */

/* Borrows the initialized characters until string is mutated or deinitialized.
 */
str_t string_view(const string_t *string);

/* Mutably borrows the initialized characters until string is mutated or
 * deinitialized. */
str_mut_t string_view_mut(string_t *string);

/* Capacity */

/* Ensures at least capacity bytes of storage without changing the string on
 * failure. */
bool string_reserve(string_t *string, usize capacity);

/* Mutation */

/*
 * Appends suffix, growing through the stored allocator when needed.
 *
 * suffix must not alias storage that growth could invalidate. The string is
 * unchanged when allocation fails.
 */
bool string_append(string_t *string, str_t suffix);

/* Appends one character, leaving the string unchanged on allocation failure. */
bool string_append_char(string_t *string, char c);

/* Removes all characters while retaining the backing allocation. */
void string_clear(string_t *string);

/* Explicit allocation/copy */

/*
 * Allocates and copies source into an empty or uninitialized out value.
 *
 * On failure, out is untouched. out must not own a live resource because a
 * successful clone overwrites it without deinitializing it.
 */
bool string_clone(allocator_t allocator, str_t source, string_t *out);
