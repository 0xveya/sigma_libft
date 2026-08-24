#include <sigma/bytes.h>
#include <sigma/qol.h>

/* sigma:begin
name: libft.bytes.basic.bytes_is_empty
provides: bytes.is_empty
deps:
externals:
kind: function
*/
bool bytes_is_empty(bytes_t bytes) { return bytes.len == 0; }
/* sigma:end */

/* sigma:begin
name: libft.bytes.basic.bytes_sub
provides: bytes.sub
deps:
externals:
kind: function
*/
bytes_t bytes_sub(bytes_t bytes, usize start, usize len) {
  if (start > bytes.len)
    start = bytes.len;

  usize remaining = bytes.len - start;

  if (len > remaining)
    len = remaining;

  return (bytes_t){
      .items = bytes.items != NULL ? bytes.items + start : NULL,
      .len = len,
  };
}
/* sigma:end */

/* sigma:begin
name: libft.bytes.basic.bytes_mut_sub
provides: bytes.mut_sub
deps:
externals:
kind: function
*/
bytes_mut_t bytes_mut_sub(bytes_mut_t bytes, usize start, usize len) {
  if (start > bytes.len)
    start = bytes.len;

  usize remaining = bytes.len - start;

  if (len > remaining)
    len = remaining;

  return (bytes_mut_t){
      .items = bytes.items != NULL ? bytes.items + start : NULL,
      .len = len,
  };
}
/* sigma:end */
