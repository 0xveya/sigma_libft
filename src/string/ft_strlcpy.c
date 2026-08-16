#include <sigma/libft.h>

/* sigma:begin
name: libft.string.ft_strlcpy.ft_strlcpy
provides: libft.ft_strlcpy
deps:
externals:
kind: function
*/
size_t ft_strlcpy(char *dst, const char *src, size_t size) {
  unsigned int i;
  unsigned int srclen;

  i = 0;
  srclen = 0;
  while (src[srclen])
    srclen++;
  if (size < 1)
    return (srclen);
  while (src[i] && i < size - 1) {
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0';
  return (srclen);
}
/* sigma:end */
