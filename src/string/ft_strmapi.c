#include <sigma/libft.h>

/* sigma:begin
name: libft.string.ft_strmapi.ft_strmapi
provides: libft.ft_strmapi
deps:
externals:
kind: function
*/
char *ft_strmapi(char const *s, char (*f)(unsigned int, char)) {
  char *str;
  int i;

  i = 0;
  str = malloc(ft_strlen(s) + 1);
  if (!str)
    return (NULL);
  while (s[i]) {
    str[i] = f((unsigned int)i, s[i]);
    i++;
  }
  str[i] = '\0';
  return (str);
}
/* sigma:end */
