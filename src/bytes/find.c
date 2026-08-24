#include <sigma/bytes.h>
#include <sigma/mem.h>

/* sigma:begin
name: libft.bytes.find.bytes_find
provides: bytes.find
deps: mem.find
externals:
kind: function
*/
usize bytes_find(bytes_t bytes, u8 value) {
  return mem_find(bytes.items, bytes.len, value);
}
/* sigma:end */

/* sigma:begin
name: libft.bytes.find.bytes_rfind
provides: bytes.rfind
deps:
externals:
kind: function
*/
usize bytes_rfind(bytes_t bytes, u8 value) {
  for (usize i = bytes.len; i > 0; --i) {
    if (bytes.items[i - 1] == value)
      return i - 1;
  }

  return SIGMA_NPOS;
}
/* sigma:end */
