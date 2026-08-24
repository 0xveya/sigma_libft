#include <sigma/qol.h>
#include <sigma/str.h>

/* sigma:begin
name: libft.str.compare.str_eq
provides: str.eq
deps:
externals:
kind: function
*/
bool str_eq(str_t a, str_t b) {
  if (a.len != b.len)
    return false;

  for (usize i = 0; i < a.len; i++) {
    if (a.items[i] != b.items[i])
      return false;
  }

  return true;
}
/* sigma:end */

/* sigma:begin
name: libft.str.compare.str_cmp
provides: str.cmp
deps:
externals:
kind: function
*/
i32 str_cmp(str_t a, str_t b) {
  usize len = a.len < b.len ? a.len : b.len;

  for (usize i = 0; i < len; i++) {
    u8 ac = (u8)a.items[i];
    u8 bc = (u8)b.items[i];

    if (ac != bc)
      return (i32)ac - (i32)bc;
  }

  if (a.len < b.len)
    return -1;

  if (a.len > b.len)
    return 1;

  return 0;
}
/* sigma:end */

/* sigma:begin
name: libft.str.compare.str_starts_with
provides: str.starts_with
deps: str.eq, str.sub
externals:
kind: function
*/
bool str_starts_with(str_t s, str_t prefix) {
  if (prefix.len > s.len)
    return false;

  return str_eq(str_sub(s, 0, prefix.len), prefix);
}
/* sigma:end */

/* sigma:begin
name: libft.str.compare.str_ends_with
provides: str.ends_with
deps: str.eq, str.sub
externals:
kind: function
*/
bool str_ends_with(str_t s, str_t suffix) {
  if (suffix.len > s.len)
    return false;

  return str_eq(str_sub(s, s.len - suffix.len, suffix.len), suffix);
}
/* sigma:end */
