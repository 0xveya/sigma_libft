#include <sigma/str.h>

/* sigma:begin
name: libft.str.find.str_find_byte
provides: str.find_byte
deps: bytes.find, str.bytes
externals:
kind: function
*/
usize str_find_byte(str_t s, u8 byte) { return bytes_find(str_bytes(s), byte); }
/* sigma:end */

/* sigma:begin
name: libft.str.find.str_rfind_byte
provides: str.rfind_byte
deps: bytes.rfind, str.bytes
externals:
kind: function
*/
usize str_rfind_byte(str_t s, u8 byte) {
  return bytes_rfind(str_bytes(s), byte);
}
/* sigma:end */
