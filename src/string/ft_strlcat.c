#include <sigma/libft.h>

/* sigma:begin
name: libft.string.ft_strlcat.ft_strlcat
provides: libft.ft_strlcat
deps:
externals:
kind: function
*/
size_t ft_strlcat(char *dst, const char *src, size_t size) {
  size_t i;
  size_t srclen;
  size_t dstlen;

  srclen = 0;
  dstlen = 0;
  while (dstlen < size && dst[dstlen])
    dstlen++;
  while (src[srclen])
    srclen++;
  if (size <= dstlen)
    return (srclen + size);
  i = 0;
  while (src[i] && dstlen + i < size - 1) {
    dst[dstlen + i] = src[i];
    i++;
  }
  dst[dstlen + i] = '\0';
  return (dstlen + srclen);
}
/* sigma:end */
