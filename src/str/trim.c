#include <sigma/ascii.h>
#include <sigma/qol.h>
#include <sigma/str.h>

/* sigma:begin
name: libft.str.trim.str_trim_start_ascii
provides: str.trim_start_ascii
deps: ascii.is_space, str.sub
externals:
kind: function
*/
str_t str_trim_start_ascii(str_t s) {
  usize start = 0;

  while (start < s.len && ascii_is_space((u8)s.items[start]))
    start++;

  return str_sub(s, start, s.len - start);
}
/* sigma:end */

/* sigma:begin
name: libft.str.trim.str_trim_end_ascii
provides: str.trim_end_ascii
deps: ascii.is_space, str.sub
externals:
kind: function
*/
str_t str_trim_end_ascii(str_t s) {
  usize end = s.len;

  while (end > 0 && ascii_is_space((u8)s.items[end - 1]))
    end--;

  return str_sub(s, 0, end);
}
/* sigma:end */

/* sigma:begin
name: libft.str.trim.str_trim_ascii
provides: str.trim_ascii
deps: str.trim_start_ascii, str.trim_end_ascii
externals:
kind: function
*/
str_t str_trim_ascii(str_t s) {
  return str_trim_end_ascii(str_trim_start_ascii(s));
}
/* sigma:end */
