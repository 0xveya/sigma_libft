#include <sigma/libft.h>

/* sigma:begin
name: libft.memcpy
provides: mem.copy
deps:
externals:
kind: function
*/
void *ft_memcpy(void *restrict dst, const void *restrict src, usize size) {
  u8 *out = dst;
  const u8 *in = src;

  for (usize i = 0; i < size; i++)
    out[i] = in[i];
  return dst;
}
/* sigma:end */
