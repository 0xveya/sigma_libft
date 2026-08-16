/* sigma:begin
name: libft.string.ft_strdup
provides: libft.string.ft_strdup
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

char *ft_strdup(const char *s) {
  char *ptr;
  size_t len;

  len = 0;
  while (s[len])
    len++;
  ptr = malloc(len + 1);
  if (!ptr)
    return (NULL);
  ft_memcpy(ptr, s, len + 1);
  return (ptr);
}

/* sigma:end */
