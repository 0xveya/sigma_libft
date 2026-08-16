/* sigma:begin
name: libft.memory.ft_memchr
provides: libft.memory.ft_memchr
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void *ft_memchr(const void *s, int c, size_t n) {
  while (n--)
    if (*(unsigned char *)s++ == (unsigned char)c)
      return ((void *)s - 1);
  return (NULL);
}

/* sigma:end */
