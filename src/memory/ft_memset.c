#include <sigma/libft.h>

/* sigma:begin
name: libft.memset
provides: mem.set
deps:
externals:
kind: function
*/
void *ft_memset(void *ptr, int byte, usize size) {
  u8 *out = ptr;

  for (usize i = 0; i < size; i++)
    out[i] = (u8)byte;
  return ptr;
}
/* sigma:end */
