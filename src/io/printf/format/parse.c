#include "../printf_internal.h"

static int ft_parse_flags(const char *s, int i, t_format *f) {
  uint32_t flag;

  flag = ft_char_to_flag(s[i]);
  while (flag != 0) {
    f->flags |= flag;
    i++;
    flag = ft_char_to_flag(s[i]);
  }
  return (i);
}

static int ft_parse_width(const char *s, int i, t_format *f) {
  while (s[i] >= '0' && s[i] <= '9') {
    f->width = f->width * 10 + (s[i] - '0');
    i++;
  }
  return (i);
}

static int ft_parse_precision(const char *s, int i, t_format *f) {
  if (s[i] == '.') {
    f->precision = 0;
    i++;
    while (s[i] >= '0' && s[i] <= '9') {
      f->precision = f->precision * 10 + (s[i] - '0');
      i++;
    }
  }
  return (i);
}

static int ft_parse_type(const char *s, int i, t_format *f) {
  f->type = ft_char_to_conv(s[i]);
  if (f->type != conv_none)
    i++;
  return (i);
}

/* sigma:begin
name: libft.io.printf.format.parse.ft_parse_format
provides: libft.ft_parse_format
deps:
externals:
kind: function
*/
int ft_parse_format(const char *s, int i, t_format *f) {
  i = ft_parse_flags(s, i, f);
  i = ft_parse_width(s, i, f);
  i = ft_parse_precision(s, i, f);
  i = ft_parse_type(s, i, f);
  return (i);
}
/* sigma:end */
