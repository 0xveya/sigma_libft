#include "format_internal.h"

static usize sigma_fmt_bytes_len(bytes_t value, const sigma_fmt_spec *spec) {
  usize len = spec->has_precision && spec->precision < value.len
                  ? spec->precision
                  : value.len;

  switch (spec->presentation) {
  case SIGMA_FMT_PRESENT_DEFAULT:
    return len;
  case SIGMA_FMT_PRESENT_HEX_LOWER:
  case SIGMA_FMT_PRESENT_HEX_UPPER:
    return len * 2;
  case SIGMA_FMT_PRESENT_BINARY:
    return len * 8;
  case SIGMA_FMT_PRESENT_DEBUG:
    return len == 0 ? 2 : len * 6;
  default:
    return SIGMA_NPOS;
  }
}

static bool sigma_fmt_write_byte_digits(sigma_writer writer, u8 value,
                                        u32 shift, const char *digits) {
  char buffer[8];
  usize len = 8 / shift;

  for (usize index = 0; index < len; index++) {
    usize offset = len - index - 1;
    buffer[index] = digits[(value >> (offset * shift)) & ((1U << shift) - 1)];
  }
  return sigma_writer_str(writer, (str_t){.items = buffer, .len = len});
}

static bool sigma_fmt_write_bytes_body(sigma_writer writer, bytes_t value,
                                       const sigma_fmt_spec *spec) {
  usize len = spec->has_precision && spec->precision < value.len
                  ? spec->precision
                  : value.len;
  const char *hex = spec->presentation == SIGMA_FMT_PRESENT_HEX_UPPER
                        ? "0123456789ABCDEF"
                        : "0123456789abcdef";

  if (spec->presentation == SIGMA_FMT_PRESENT_DEFAULT)
    return sigma_writer_write(writer,
                              (bytes_t){.items = value.items, .len = len});
  if (spec->presentation == SIGMA_FMT_PRESENT_DEBUG &&
      !sigma_writer_str(writer, STR_LIT("[")))
    return false;
  for (usize index = 0; index < len; index++) {
    if (spec->presentation == SIGMA_FMT_PRESENT_DEBUG) {
      if (index != 0 && !sigma_writer_str(writer, STR_LIT(", ")))
        return false;
      if (!sigma_writer_str(writer, STR_LIT("0x")) ||
          !sigma_fmt_write_byte_digits(writer, value.items[index], 4, hex))
        return false;
    } else if (!sigma_fmt_write_byte_digits(
                   writer, value.items[index],
                   spec->presentation == SIGMA_FMT_PRESENT_BINARY ? 1 : 4,
                   spec->presentation == SIGMA_FMT_PRESENT_BINARY ? "01" : hex))
      return false;
  }
  return spec->presentation != SIGMA_FMT_PRESENT_DEBUG ||
         sigma_writer_str(writer, STR_LIT("]"));
}

/* sigma:begin
name: libft.io.printf.format.bytes.sigma_fmt_write_bytes
provides: libft.sigma_fmt_write_bytes
deps: libft.sigma_writer_write libft.sigma_writer_str
libft.sigma_fmt_write_repeat externals: kind: function
*/
bool sigma_fmt_write_bytes(sigma_writer writer, bytes_t value,
                           const sigma_fmt_spec *spec) {
  if (spec->alternate || spec->zero_pad || spec->sign_plus || spec->sign_space)
    return false;

  usize len = sigma_fmt_bytes_len(value, spec);
  if (len == SIGMA_NPOS)
    return false;
  usize pad = spec->has_width && spec->width > len ? spec->width - len : 0;
  sigma_fmt_align align = spec->align == SIGMA_FMT_ALIGN_DEFAULT
                              ? SIGMA_FMT_ALIGN_LEFT
                              : spec->align;
  usize left = align == SIGMA_FMT_ALIGN_LEFT     ? 0
               : align == SIGMA_FMT_ALIGN_CENTER ? pad / 2
                                                 : pad;

  return sigma_fmt_write_repeat(writer, spec->fill, left) &&
         sigma_fmt_write_bytes_body(writer, value, spec) &&
         sigma_fmt_write_repeat(writer, spec->fill, pad - left);
}
/* sigma:end */
