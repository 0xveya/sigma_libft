#include "../printf_internal.h"

/* sigma:begin
name: libft.io.printf.format.parse_utils.ft_format_has
provides: libft.ft_format_has
deps:
externals:
kind: function
*/
int ft_format_has(const t_format *f, uint32_t flag) {
  return ((f->flags & flag) != 0);
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.format.parse_utils.ft_format_init
provides: libft.ft_format_init
deps:
externals:
kind: function
*/
void ft_format_init(t_format *f) {
  *f = (t_format){0};
  f->precision = -1;
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.format.parse_utils.ft_char_to_flag
provides: libft.ft_char_to_flag
deps:
externals:
kind: function
*/
uint32_t ft_char_to_flag(char c) {
  if (c == '-')
    return (FMT_MINUS);
  if (c == '0')
    return (FMT_ZERO);
  if (c == '#')
    return (FMT_HASH);
  if (c == '+')
    return (FMT_PLUS);
  if (c == ' ')
    return (FMT_SPACE);
  return (0);
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.format.parse_utils.ft_char_to_conv
provides: libft.ft_char_to_conv
deps:
externals:
kind: function
*/
t_conv ft_char_to_conv(char c) {
  if (c == 'c')
    return (conv_char);
  if (c == 's')
    return (conv_str);
  if (c == 'p')
    return (conv_ptr);
  if (c == 'd' || c == 'i')
    return (conv_int);
  if (c == 'u')
    return (conv_uint);
  if (c == 'x')
    return (conv_hex_low);
  if (c == 'X')
    return (conv_hex_up);
  if (c == '%')
    return (conv_percent);
  return (conv_none);
}
/* sigma:end */
