#include <sigma/libft.h>

/** Performs the ft memmove operation. */
void *ft_memmove(void *dst, const void *src, size_t len) {
  unsigned char *dest_char;
  unsigned char *src_char;

  dest_char = (unsigned char *)dst;
  src_char = (unsigned char *)src;
  if (!dst && !src)
    return (dst);
  if (src < dst)
    while (len--)
      dest_char[len] = src_char[len];
  else
    while (len--)
      *dest_char++ = *src_char++;
  return (dst);
}
