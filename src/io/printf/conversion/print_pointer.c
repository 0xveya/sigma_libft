#include "../printf_internal.h"

/* sigma:begin
name: libft.io.printf.conversion.print_pointer.ft_print_ptr_fmt
provides: libft.ft_print_ptr_fmt
deps:
externals:
kind: function
*/
int ft_print_ptr_fmt(t_writer *w, void *ptr, t_format *f) {
  char buf[2 * sizeof(uintptr_t)];
  char *begin;
  char *end;

  (void)f;
  if (!ptr)
    return (ft_writer_write(w, "(nil)", 5));
  ft_writer_write(w, "0x", 2);
  end = buf + sizeof(buf);
  begin = ft_u64_pow2(end, (uint64_t)(uintptr_t)ptr, 4, "0123456789abcdef");
  ft_writer_write(w, begin, (size_t)(end - begin));
  return (!w->error);
}
/* sigma:end */
