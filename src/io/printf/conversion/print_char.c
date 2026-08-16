#include "../printf_internal.h"

/* sigma:begin
name: libft.io.printf.conversion.print_char.ft_print_char_fmt
provides: libft.ft_print_char_fmt
deps:
externals:
kind: function
*/
int ft_print_char_fmt(t_writer *w, int c, t_format *f) {
  int pad;

  pad = f->width - 1;
  if (pad < 0)
    pad = 0;
  if (!ft_format_has(f, FMT_MINUS))
    ft_writer_repeat(w, ' ', (size_t)pad);
  ft_writer_char(w, (char)c);
  if (ft_format_has(f, FMT_MINUS))
    ft_writer_repeat(w, ' ', (size_t)pad);
  return (!w->error);
}
/* sigma:end */
