/* sigma:begin
name: libft.memory.ft_memchr
provides: libft.memory.ft_memchr
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void *ft_memchr(const void *s, int c, size_t n) {
  const unsigned char *bytes = s;

  while (n--) {
    if (*bytes == (unsigned char)c)
      return (void *)bytes;
    bytes++;
  }
  return (NULL);
}

/* sigma:end */
