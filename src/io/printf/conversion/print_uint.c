#include "../printf_internal.h"

/* sigma:begin
name: libft.io.printf.conversion.print_uint.ft_print_uint_fmt
provides: libft.ft_print_uint_fmt
deps:
externals:
kind: function
*/
int ft_print_uint_fmt(t_writer *w, unsigned int n, t_format *f) {
  char buf[32];
  char *begin;
  char *end;

  (void)f;
  end = buf + sizeof(buf);
  begin = ft_u64_dec(end, (uint64_t)n);
  return (ft_writer_write(w, begin, (size_t)(end - begin)));
}
/* sigma:end */
