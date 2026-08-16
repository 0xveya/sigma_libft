#include "../printf_internal.h"

static void ft_prepare_value(t_intfmt *v, int n, t_format *f) {
  int64_t value;

  value = (int64_t)n;
  v->sign = 0;
  if (value < 0) {
    v->sign = '-';
    v->magnitude = (uint64_t)(-value);
  } else {
    v->magnitude = (uint64_t)value;
    if (ft_format_has(f, FMT_PLUS))
      v->sign = '+';
    else if (ft_format_has(f, FMT_SPACE))
      v->sign = ' ';
  }
}

static void ft_prepare_layout(t_intfmt *v, t_format *f, int digits_len) {
  v->digits_len = digits_len;
  if (f->precision == 0 && v->magnitude == 0)
    v->digits_len = 0;
  v->zeroes = 0;
  if (f->precision > v->digits_len)
    v->zeroes = f->precision - v->digits_len;
  v->pad = f->width - v->digits_len - v->zeroes - (v->sign != 0);
  if (v->pad < 0)
    v->pad = 0;
}

static void ft_write_int_body(t_writer *w, t_intfmt *v, const char *digits) {
  if (v->sign)
    ft_writer_char(w, (char)v->sign);
  ft_writer_repeat(w, '0', (size_t)v->zeroes);
  if (v->digits_len != 0)
    ft_writer_write(w, digits, (size_t)v->digits_len);
}

/* sigma:begin
name: libft.io.printf.conversion.print_int.ft_print_int_fmt
provides: libft.ft_print_int_fmt
deps:
externals:
kind: function
*/
int ft_print_int_fmt(t_writer *w, int n, t_format *f) {
  t_intfmt v;
  char buf[32];
  char *digits;
  char *end;

  end = buf + sizeof(buf);
  ft_prepare_value(&v, n, f);
  digits = ft_u64_dec(end, v.magnitude);
  ft_prepare_layout(&v, f, (int)(end - digits));
  if (!ft_format_has(f, FMT_MINUS) && ft_format_has(f, FMT_ZERO) &&
      f->precision < 0) {
    v.zeroes += v.pad;
    v.pad = 0;
  }
  if (!ft_format_has(f, FMT_MINUS))
    ft_writer_repeat(w, ' ', (size_t)v.pad);
  ft_write_int_body(w, &v, digits);
  if (ft_format_has(f, FMT_MINUS))
    ft_writer_repeat(w, ' ', (size_t)v.pad);
  return (!w->error);
}
/* sigma:end */
