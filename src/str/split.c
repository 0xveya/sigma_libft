#include <sigma/str.h>

/* sigma:begin
name: libft.str.split.str_split_scalar
provides: str.split_scalar
deps:
externals:
kind: function
*/
str_split_scalar_t str_split_scalar(str_t s, u8 delimiter) {
  return (str_split_scalar_t){
      .remaining = s,
      .delimiter = delimiter,
      .finished = false,
  };
}
/* sigma:end */

/* sigma:begin
name: libft.str.split.str_split_scalar_next
provides: str.split_scalar.next
deps: str.find_byte, str.sub
externals:
kind: function
*/
bool str_split_scalar_next(str_split_scalar_t *it, str_t *out) {
  if (it->finished)
    return false;

  usize pos = str_find_byte(it->remaining, it->delimiter);

  if (pos == SIGMA_NPOS) {
    *out = it->remaining;
    it->remaining = (str_t){0};
    it->finished = true;
    return true;
  }
  *out = str_sub(it->remaining, 0, pos);

  it->remaining = str_sub(it->remaining, pos + 1, it->remaining.len);

  return true;
}
/* sigma:end */
