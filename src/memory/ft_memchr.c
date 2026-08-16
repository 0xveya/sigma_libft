#include <sigma/libft.h>

/* sigma:begin
name: libft.memory.ft_memchr.ft_memchr
provides: libft.ft_memchr
deps:
externals:
kind: function
*/
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
