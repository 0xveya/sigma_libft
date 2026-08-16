#include <sigma/libft.h>

/* sigma:begin
name: libft.string.ft_strjoin.ft_strjoin
provides: libft.ft_strjoin
deps:
externals:
kind: function
*/
char *ft_strjoin(char const *s1, char const *s2) {
  char *ptr;
  size_t s1_len;
  size_t s2_len;

  s1_len = ft_strlen(s1);
  s2_len = ft_strlen(s2);
  ptr = malloc(s1_len + s2_len + 1);
  if (!ptr)
    return (NULL);
  if (s1_len > 0)
    ft_memcpy(ptr, s1, s1_len);
  if (s2_len > 0)
    ft_memcpy(ptr + s1_len, s2, s2_len);
  return (ptr[s1_len + s2_len] = '\0', ptr);
}
/* sigma:end */
