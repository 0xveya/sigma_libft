#include <sigma/str.h>

/** Performs the str from cstr operation. */
str_t str_from_cstr(const char *s) {
  usize len = 0;
  while (s[len]) {
    len++;
  }
  return (str_t){.items = s, .len = len};
}

/** Performs the str to cstr operation. */
char *str_to_cstr(str_t s, allocator_t allocator) {
  if (s.len == SIGMA_NPOS)
    return NULL;

  char *result = allocator_array(allocator, char, s.len + 1);
  if (result == NULL)
    return NULL;

  for (usize i = 0; i < s.len; i++)
    result[i] = s.items[i];
  result[s.len] = '\0';
  return result;
}

/** Performs the str is empty operation. */
bool str_is_empty(str_t s) { return s.len == 0; }

/** Performs the str sub operation. */
str_t str_sub(str_t s, usize start, usize len) {
  if (start > s.len)
    start = s.len;

  usize remaining = s.len - start;
  if (len > remaining)
    len = remaining;

  return (str_t){.items = s.items + start, .len = len};
}
