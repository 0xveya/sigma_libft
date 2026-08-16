/* sigma:begin
name: libft.string.ft_strchr
provides: libft.string.ft_strchr
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

char *ft_strchr(const char *s, int c) {
  while (*s)
    if (*s++ == (char)c)
      return ((char *)s - 1);
  if (c == '\0')
    return ((char *)s);
  return (NULL);
}

/* sigma:end */
