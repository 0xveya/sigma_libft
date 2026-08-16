/* sigma:begin
name: libft.string.ft_strtrim
provides: libft.string.ft_strtrim
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

static int ft_isset(char c, char const *set) {
  size_t i;

  i = 0;
  while (set[i]) {
    if (set[i] == c)
      return (1);
    i++;
  }
  return (0);
}

char *ft_strtrim(char const *s1, char const *set) {
  size_t start;
  size_t end;
  char *trimmed;
  size_t i;

  if (!s1 || !set)
    return (NULL);
  start = 0;
  while (s1[start] && ft_isset(s1[start], set))
    start++;
  end = ft_strlen(s1);
  while (end > start && ft_isset(s1[end - 1], set))
    end--;
  trimmed = malloc(sizeof(char) * (end - start + 1));
  if (!trimmed)
    return (NULL);
  i = 0;
  while (start < end)
    trimmed[i++] = s1[start++];
  trimmed[i] = '\0';
  return (trimmed);
}

/* sigma:end */
