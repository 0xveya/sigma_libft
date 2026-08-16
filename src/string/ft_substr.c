#include <sigma/libft.h>

/* sigma:begin
name: libft.string.ft_substr.ft_substr
provides: libft.ft_substr
deps:
externals:
kind: function
*/
char *ft_substr(char const *s, unsigned int start, size_t len) {
  char *ptr;
  size_t s_len;

  if (!s)
    return (NULL);
  s_len = ft_strlen(s);
  if (start >= s_len)
    return (ft_strdup(""));
  if (len > s_len - start)
    len = s_len - start;
  ptr = malloc(len + 1);
  if (!ptr)
    return (NULL);
  ft_memcpy(ptr, s + start, len);
  ptr[len] = '\0';
  return (ptr);
}
/* sigma:end */
