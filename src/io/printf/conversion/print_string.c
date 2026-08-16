#include "../printf_internal.h"

/* sigma:begin
name: libft.io.printf.conversion.print_string.ft_print_str_fmt
provides: libft.ft_print_str_fmt
deps:
externals:
kind: function
*/
int ft_print_str_fmt(t_writer *w, const char *s, t_format *f) {
  int len;
  int pad;

  if (!s)
    s = "(null)";
  len = (int)ft_strlen(s);
  if (f->precision >= 0 && f->precision < len)
    len = f->precision;
  pad = f->width - len;
  if (pad < 0)
    pad = 0;
  if (!ft_format_has(f, FMT_MINUS))
    ft_writer_repeat(w, ' ', (size_t)pad);
  ft_writer_write(w, s, (size_t)len);
  if (ft_format_has(f, FMT_MINUS))
    ft_writer_repeat(w, ' ', (size_t)pad);
  return (!w->error);
}
/* sigma:end */
