#include <sigma/libft.h>

/* sigma:begin
name: libft.char.ft_to_upper_to_lower.ft_toupper
provides: libft.ft_toupper
deps:
externals:
kind: function
*/
int ft_toupper(int c) {
  if (c >= 'a' && c <= 'z')
    return (c - 32);
  else
    return (c);
}
/* sigma:end */

/* sigma:begin
name: libft.char.ft_to_upper_to_lower.ft_tolower
provides: libft.ft_tolower
deps:
externals:
kind: function
*/
int ft_tolower(int c) {
  if (c >= 'A' && c <= 'Z')
    return (c + 32);
  else
    return (c);
}
/* sigma:end */
