#include "../printf_internal.h"

/* sigma:begin
name: libft.io.printf.conversion.print_percent.ft_print_percent_fmt
provides: libft.ft_print_percent_fmt
deps:
externals:
kind: function
*/
int ft_print_percent_fmt(t_writer *w, t_format *f) {
  (void)f;
  return (ft_writer_char(w, '%'));
}
/* sigma:end */
