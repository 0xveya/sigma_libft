/* sigma:begin
name: libft.char.ft_to_upper_to_lower
provides: libft.char.ft_to_upper_to_lower
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

int ft_toupper(int c) {
  if (c >= 'a' && c <= 'z')
    return (c - 32);
  else
    return (c);
}

int ft_tolower(int c) {
  if (c >= 'A' && c <= 'Z')
    return (c + 32);
  else
    return (c);
}

/* sigma:end */
