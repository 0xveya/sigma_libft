/* sigma:begin
name: libft.io.printf.core.printf_dispatch
provides: libft.io.printf.core.printf_dispatch
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

int ft_dispatch_print(t_writer *w, t_format *f, va_list args) {
  if (f->type == conv_char)
    return (ft_print_char_fmt(w, va_arg(args, int), f));
  if (f->type == conv_str)
    return (ft_print_str_fmt(w, va_arg(args, char *), f));
  if (f->type == conv_ptr)
    return (ft_print_ptr_fmt(w, va_arg(args, void *), f));
  if (f->type == conv_int)
    return (ft_print_int_fmt(w, va_arg(args, int), f));
  if (f->type == conv_uint)
    return (ft_print_uint_fmt(w, va_arg(args, unsigned int), f));
  if (f->type == conv_hex_low)
    return (ft_print_hex_low_fmt(w, va_arg(args, unsigned int), f));
  if (f->type == conv_hex_up)
    return (ft_print_hex_up_fmt(w, va_arg(args, unsigned int), f));
  if (f->type == conv_percent)
    return (ft_print_percent_fmt(w, f));
  return (0);
}
/* sigma:end */
