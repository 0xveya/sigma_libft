#include "format_internal.h"

typedef struct {
  char *digits;
  usize digits_len;
  str_t prefix;
  char sign;
} sigma_fmt_integer;

[[gnu::always_inline]] static inline const char *sigma_fmt_digits100(void) {
  return ("00010203040506070809"
          "10111213141516171819"
          "20212223242526272829"
          "30313233343536373839"
          "40414243444546474849"
          "50515253545556575859"
          "60616263646566676869"
          "70717273747576777879"
          "80818283848586878889"
          "90919293949596979899");
}

static char *sigma_fmt_u64_dec(char *end, u64 value) {
  const char *digits100 = sigma_fmt_digits100();

  if (value == 0)
    *--end = '0';
  while (value >= 100) {
    u32 pair = (u32)(value % 100);

    value /= 100;
    end -= 2;
    end[0] = digits100[pair * 2];
    end[1] = digits100[pair * 2 + 1];
  }
  if (value > 0 && value < 10)
    *--end = (char)('0' + value);
  else if (value >= 10) {
    u32 pair = (u32)value;

    end -= 2;
    end[0] = digits100[pair * 2];
    end[1] = digits100[pair * 2 + 1];
  }
  return end;
}

static char *sigma_fmt_u64_pow2(char *end, u64 value, u32 shift,
                                const char *digits) {
  u64 mask = (UINT64_C(1) << shift) - 1;

  if (value == 0)
    *--end = '0';
  while (value != 0) {
    *--end = digits[value & mask];
    value >>= shift;
  }
  return end;
}

static sigma_fmt_integer sigma_fmt_render_integer(char *end, u64 value,
                                                  char sign,
                                                  const sigma_fmt_spec *spec) {
  sigma_fmt_integer integer = {.prefix = STR_LIT(""), .sign = sign};

  switch (spec->presentation) {
  case SIGMA_FMT_PRESENT_DEFAULT:
  case SIGMA_FMT_PRESENT_DEC:
    integer.digits = sigma_fmt_u64_dec(end, value);
    break;
  case SIGMA_FMT_PRESENT_HEX_LOWER:
    integer.digits = sigma_fmt_u64_pow2(end, value, 4, "0123456789abcdef");
    if (spec->alternate)
      integer.prefix = STR_LIT("0x");
    break;
  case SIGMA_FMT_PRESENT_HEX_UPPER:
    integer.digits = sigma_fmt_u64_pow2(end, value, 4, "0123456789ABCDEF");
    if (spec->alternate)
      integer.prefix = STR_LIT("0X");
    break;
  case SIGMA_FMT_PRESENT_BINARY:
    integer.digits = sigma_fmt_u64_pow2(end, value, 1, "01");
    if (spec->alternate)
      integer.prefix = STR_LIT("0b");
    break;
  case SIGMA_FMT_PRESENT_OCTAL:
    integer.digits = sigma_fmt_u64_pow2(end, value, 3, "01234567");
    if (spec->alternate)
      integer.prefix = STR_LIT("0o");
    break;
  case SIGMA_FMT_PRESENT_POINTER:
  case SIGMA_FMT_PRESENT_DEBUG:
  default:
    panic("this integer presentation is not implemented");
  }
  integer.digits_len = (usize)(end - integer.digits);
  return integer;
}

static bool sigma_fmt_write_integer(sigma_writer writer,
                                    sigma_fmt_integer integer,
                                    const sigma_fmt_spec *spec) {
  usize zeroes = spec->has_precision && spec->precision > integer.digits_len
                     ? spec->precision - integer.digits_len
                     : 0;
  usize body_len = integer.digits_len + integer.prefix.len + zeroes +
                   (usize)(integer.sign != '\0');
  usize pad =
      spec->has_width && spec->width > body_len ? spec->width - body_len : 0;
  sigma_fmt_align align = spec->align == SIGMA_FMT_ALIGN_DEFAULT
                              ? SIGMA_FMT_ALIGN_RIGHT
                              : spec->align;
  usize left = align == SIGMA_FMT_ALIGN_LEFT     ? 0
               : align == SIGMA_FMT_ALIGN_CENTER ? pad / 2
                                                 : pad;
  usize right = pad - left;

  if (spec->zero_pad && !spec->has_precision &&
      align == SIGMA_FMT_ALIGN_RIGHT) {
    zeroes += left;
    left = 0;
  }
  if (!sigma_fmt_write_repeat(writer, spec->fill, left))
    return false;
  if (integer.sign != '\0' &&
      !sigma_writer_str(writer, (str_t){.items = &integer.sign, .len = 1}))
    return false;
  return sigma_writer_str(writer, integer.prefix) &&
         sigma_fmt_write_repeat(writer, '0', zeroes) &&
         sigma_writer_str(writer, (str_t){.items = integer.digits,
                                          .len = integer.digits_len}) &&
         sigma_fmt_write_repeat(writer, spec->fill, right);
}

/** Performs the sigma fmt write u64 operation. */
bool sigma_fmt_write_u64(sigma_writer writer, u64 value,
                         const sigma_fmt_spec *spec) {
  char buffer[64];
  char sign = spec->sign_plus ? '+' : spec->sign_space ? ' ' : '\0';
  sigma_fmt_integer integer =
      sigma_fmt_render_integer(buffer + sizeof(buffer), value, sign, spec);

  return sigma_fmt_write_integer(writer, integer, spec);
}

/** Performs the sigma fmt write i64 operation. */
bool sigma_fmt_write_i64(sigma_writer writer, i64 value,
                         const sigma_fmt_spec *spec) {
  char buffer[64];
  char sign = '\0';
  u64 magnitude;

  if (value < 0) {
    sign = '-';
    magnitude = (u64)(-(value + 1)) + 1;
  } else {
    magnitude = (u64)value;
    sign = spec->sign_plus ? '+' : spec->sign_space ? ' ' : '\0';
  }
  sigma_fmt_integer integer =
      sigma_fmt_render_integer(buffer + sizeof(buffer), magnitude, sign, spec);
  return sigma_fmt_write_integer(writer, integer, spec);
}
