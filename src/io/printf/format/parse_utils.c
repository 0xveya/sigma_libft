/* sigma:begin
name: libft.io.printf.format.parse_utils
provides: libft.io.printf.format.parse_utils
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

int ft_format_has(const t_format *f, uint32_t flag) {
  return ((f->flags & flag) != 0);
}

void ft_format_init(t_format *f) {
  *f = (t_format){0};
  f->precision = -1;
}

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
