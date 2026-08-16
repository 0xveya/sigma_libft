#include <sigma/libft.h>

/* sigma:begin
name: libft.string.ft_strrchr.ft_strrchr
provides: libft.ft_strrchr
deps:
externals:
kind: function
*/
char *ft_strrchr(const char *s, int c) {
  const char *str;

  str = s;
  while (*s)
    s++;
  while (s >= str)
    if (*s-- == (char)c)
      return ((char *)s + 1);
  return (NULL);
}
/* sigma:end */
