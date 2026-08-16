/* sigma:begin
name: libft.string.ft_strnstr
provides: libft.string.ft_strnstr
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

char *ft_strnstr(const char *str, const char *to_find, size_t n) {
  size_t i;
  size_t j;

  if (!to_find[0])
    return ((char *)str);
  i = 0;
  while (str[i] && i < n) {
    j = 0;
    while (to_find[j] && i + j < n && str[i + j] == to_find[j])
      j++;
    if (!to_find[j])
      return ((char *)str + i);
    i++;
  }
  return (NULL);
}

/* sigma:end */
