#include <sigma/str.h>

/* sigma:begin
name: libft.str.compare.str_eq
provides: str.eq
deps: bytes.eq, str.bytes
externals:
kind: function
*/
bool str_eq(str_t a, str_t b) { return bytes_eq(str_bytes(a), str_bytes(b)); }
/* sigma:end */

/* sigma:begin
name: libft.str.compare.str_cmp
provides: str.cmp
deps: bytes.cmp, str.bytes
externals:
kind: function
*/
i32 str_cmp(str_t a, str_t b) { return bytes_cmp(str_bytes(a), str_bytes(b)); }
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
