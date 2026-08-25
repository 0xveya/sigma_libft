#include <sigma/mem.h>
#include <sigma/ownership.h>
#include <sigma/string.h>
#include <stdint.h>

static bool ensure_capacity(string_t *string, usize additional) {
  if (additional > SIZE_MAX - string->len)
    return false;

  usize required = string->len + additional;

  if (required <= string->cap)
    return true;

  usize capacity = string->cap == 0 ? 8 : string->cap;

  while (capacity < required) {
    if (capacity > SIZE_MAX / 2) {
      capacity = required;
      break;
    }

    capacity *= 2;
  }

  return string_reserve(string, capacity);
}

string_t string_init(allocator_t allocator) {
  return (string_t){
      .items = NULL,
      .len = 0,
      .cap = 0,
      .allocator = allocator,
  };
}

void string_deinit(string_t *string) {
  if (string->items != NULL) {
    allocator_free_aligned(string->allocator, string->items,
                           string->cap * sizeof(char), _Alignof(char));
  }

  *string = (string_t){0};
}

string_t string_take(string_t *source) {
  string_t result = {0};
  SIGMA_MOVE_PTR(&result, source);

  return result;
}

str_t string_view(const string_t *string) {
  return (str_t){
      .items = string->items,
      .len = string->len,
  };
}

str_mut_t string_view_mut(string_t *string) {
  return (str_mut_t){
      .items = string->items,
      .len = string->len,
  };
}

void string_clear(string_t *string) { string->len = 0; }

bool string_reserve(string_t *string, usize capacity) {
  if (capacity <= string->cap)
    return true;

  if (capacity > SIZE_MAX / sizeof(char))
    return false;

  char *items = allocator_realloc_aligned_debug(
      string->allocator, string->items, string->cap * sizeof(char),
      capacity * sizeof(char), _Alignof(char), __FILE__, __func__, __LINE__);

  if (items == NULL)
    return false;

  string->items = items;
  string->cap = capacity;

  return true;
}

bool string_append(string_t *string, str_t suffix) {
  if (suffix.len == 0)
    return true;

  if (!ensure_capacity(string, suffix.len))
    return false;

  sigma_memcpy(string->items + string->len, suffix.items, suffix.len);

  string->len += suffix.len;

  return true;
}

bool string_append_char(string_t *string, char c) {
  if (!ensure_capacity(string, 1))
    return false;

  string->items[string->len++] = c;

  return true;
}

bool string_from_str(string_t *out, allocator_t allocator, str_t source) {
  string_t result = string_init(allocator);

  if (source.len != 0) {
    if (!string_reserve(&result, source.len))
      return false;

    sigma_memcpy(result.items, source.items, source.len);

    result.len = source.len;
  }

  *out = result;

  return true;
}

bool string_clone(string_t *out, const string_t *source) {
  return string_from_str(out, source->allocator, string_view(source));
}
