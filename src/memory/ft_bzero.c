/* sigma:begin
name: libft.memory.ft_bzero
provides: libft.memory.ft_bzero
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void ft_bzero(void *s, size_t n) {
  unsigned char *str;

  str = (unsigned char *)s;
  while (n--)
    *str++ = (unsigned char)0;
}
/* sigma:end */
