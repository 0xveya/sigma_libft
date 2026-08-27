#include "format_internal.h"

/* sigma:begin
name: libft.io.printf.format.str.sigma_fmt_write_str
provides: libft.sigma_fmt_write_str
deps: libft.sigma_writer_str libft.sigma_fmt_write_repeat
externals:
kind: function
*/
bool sigma_fmt_write_str(sigma_writer writer, str_t value,
                         const sigma_fmt_spec *spec) {
  if (spec->presentation != SIGMA_FMT_PRESENT_DEFAULT)
    return false;
  if (spec->alternate || spec->zero_pad || spec->sign_plus || spec->sign_space)
    return false;

  usize len = value.len;
  if (spec->has_precision && spec->precision < len)
    len = spec->precision;
  usize pad = spec->has_width && spec->width > len ? spec->width - len : 0;
  sigma_fmt_align align = spec->align == SIGMA_FMT_ALIGN_DEFAULT
                              ? SIGMA_FMT_ALIGN_LEFT
                              : spec->align;
  usize left = align == SIGMA_FMT_ALIGN_LEFT     ? 0
               : align == SIGMA_FMT_ALIGN_CENTER ? pad / 2
                                                 : pad;
  usize right = pad - left;

  return sigma_fmt_write_repeat(writer, spec->fill, left) &&
         sigma_writer_str(writer, (str_t){.items = value.items, .len = len}) &&
         sigma_fmt_write_repeat(writer, spec->fill, right);
}
/* sigma:end */
