/* sigma:begin
name: libft.char.ft_is_things.ft_isalpha
provides: libft.ft_isalpha
deps:
externals:
kind: function
*/
int ft_isalpha(int c) {
  if (c >= 'a' && c <= 'z')
    return (1);
  else if (c >= 'A' && c <= 'Z')
    return (1);
  else
    return (0);
}
/* sigma:end */

/* sigma:begin
name: libft.char.ft_is_things.ft_isdigit
provides: libft.ft_isdigit
deps:
externals:
kind: function
*/
int ft_isdigit(int c) {
  if (c >= '0' && c <= '9')
    return (1);
  else
    return (0);
}
/* sigma:end */

/* sigma:begin
name: libft.char.ft_is_things.ft_isalnum
provides: libft.ft_isalnum
deps:
externals:
kind: function
*/
int ft_isalnum(int c) {
  if (ft_isalpha(c) || ft_isdigit(c))
    return (1);
  else
    return (0);
}
/* sigma:end */

/* sigma:begin
name: libft.char.ft_is_things.ft_isascii
provides: libft.ft_isascii
deps:
externals:
kind: function
*/
int ft_isascii(int c) {
  if (c >= 0 && c <= 127)
    return (1);
  else
    return (0);
}
/* sigma:end */

/* sigma:begin
name: libft.char.ft_is_things.ft_isprint
provides: libft.ft_isprint
deps:
externals:
kind: function
*/
int ft_isprint(int c) {
  if (c >= 32 && c <= 126)
    return (1);
  else
    return (0);
}
/* sigma:end */
