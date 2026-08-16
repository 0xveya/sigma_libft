#include <sigma/mem.h>
#include <sigma/slice.h>
#include <sigma/str.h>

/* sigma:begin
name: libft.slice.str.sigma_str_from_cstr
provides: str.from_cstr
deps: str.len
externals:
kind: function
*/
str_t sigma_str_from_cstr(const char *text) {
  return (str_t){.items = (char *)text, .len = ft_strlen(text)};
}
/* sigma:end */

/* sigma:begin
name: libft.slice.str.sigma_str_eq
provides: str.eq
deps: mem.compare
externals:
kind: function
*/
bool sigma_str_eq(str_t left, str_t right) {
  return left.len == right.len &&
         ft_memcmp(left.items, right.items, left.len) == 0;
}
/* sigma:end */
