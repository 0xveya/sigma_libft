#include <sigma/qol.h>
#include <sigma/str.h>

/* sigma:begin
name: libft.str.find.str_find_byte
provides: str.find_byte
deps:
externals:
kind: function
*/
usize str_find_byte(str_t s, u8 byte) {
  for (usize i = 0; i < s.len; i++) {
    if ((u8)s.items[i] == byte)
      return i;
  }
  return STR_NPOS;
}
/* sigma:end */

/* sigma:begin
name: libft.str.find.str_rfind_byte
provides: str.rfind_byte
deps:
externals:
kind: function
*/
usize str_rfind_byte(str_t s, u8 byte) {
  for (usize i = s.len; i > 0; i--) {
    if ((u8)s.items[i - 1] == byte)
      return i - 1;
  }

  return STR_NPOS;
}
/* sigma:end */
