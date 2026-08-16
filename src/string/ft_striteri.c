/* sigma:begin
name: libft.string.ft_striteri
provides: libft.string.ft_striteri
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void ft_striteri(char *s, void (*f)(unsigned int, char *)) {
  unsigned int i;

  i = 0;
  while (s[i]) {
    f(i, s + i);
    i++;
  }
}

/* sigma:end */
