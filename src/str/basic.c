#include <sigma/str.h>

/* sigma:begin
name: libft.str.basic.str_from_cstr
provides: str.from_cstr
deps:
externals:
kind: function
*/
str_t str_from_cstr(const char *s) {
  usize len = 0;
  while (s[len]) {
    len++;
  }
  return (str_t){.items = s, .len = len};
}
/* sigma:end */

/* sigma:begin
name: libft.str.basic.str_to_cstr
provides: str.to_cstr
deps:
externals: allocator.alloc
kind: function
*/
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
/* sigma:end */

/* sigma:begin
name: libft.str.basic.str_is_empty
provides: str.is_empty
deps:
externals:
kind: function
*/
bool str_is_empty(str_t s) { return s.len == 0; }
/* sigma:end */

/* sigma:begin
name: libft.str.basic.str_sub
provides: str.sub
deps:
externals:
kind: function
*/
str_t str_sub(str_t s, usize start, usize len) {
  if (start > s.len)
    start = s.len;

  usize remaining = s.len - start;
  if (len > remaining)
    len = remaining;

  return (str_t){.items = s.items + start, .len = len};
}
/* sigma:end */
