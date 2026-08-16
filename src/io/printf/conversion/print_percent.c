/* sigma:begin
name: libft.io.printf.conversion.print_percent
provides: libft.io.printf.conversion.print_percent
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

int ft_print_percent_fmt(t_writer *w, t_format *f) {
  (void)f;
  return (ft_writer_char(w, '%'));
}
/* sigma:end */
