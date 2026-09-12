#include <sigma/libft.h>

/** Performs the ft memchr operation. */
void *ft_memchr(const void *s, int c, size_t n) {
  const unsigned char *bytes = s;

  while (n--) {
    if (*bytes == (unsigned char)c)
      return (void *)bytes;
    bytes++;
  }
  return (NULL);
}
