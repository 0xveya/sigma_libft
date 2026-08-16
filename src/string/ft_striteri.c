#include <sigma/libft.h>

/* sigma:begin
name: libft.string.ft_striteri.ft_striteri
provides: libft.ft_striteri
deps:
externals:
kind: function
*/
void ft_striteri(char *s, void (*f)(unsigned int, char *)) {
  unsigned int i;

  i = 0;
  while (s[i]) {
    f(i, s + i);
    i++;
  }
}
/* sigma:end */
