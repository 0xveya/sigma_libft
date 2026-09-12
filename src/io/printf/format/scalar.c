#include "format_internal.h"

/** Performs the sigma fmt write bool operation. */
bool sigma_fmt_write_bool(sigma_writer writer, bool value,
                          const sigma_fmt_spec *spec) {
  sigma_fmt_spec string_spec = *spec;

  if (spec->presentation != SIGMA_FMT_PRESENT_DEFAULT &&
      spec->presentation != SIGMA_FMT_PRESENT_DEBUG)
    return false;
  string_spec.presentation = SIGMA_FMT_PRESENT_DEFAULT;
  return sigma_fmt_write_str(writer, value ? STR_LIT("true") : STR_LIT("false"),
                             &string_spec);
}

/** Performs the sigma fmt write char operation. */
bool sigma_fmt_write_char(sigma_writer writer, char value,
                          const sigma_fmt_spec *spec) {
  sigma_fmt_spec string_spec = *spec;

  if (spec->presentation != SIGMA_FMT_PRESENT_DEFAULT &&
      spec->presentation != SIGMA_FMT_PRESENT_DEBUG)
    return false;
  string_spec.presentation = SIGMA_FMT_PRESENT_DEFAULT;
  return sigma_fmt_write_str(
      writer, (str_t){.items = &value, .len = sizeof(value)}, &string_spec);
}

/** Performs the sigma fmt write rune operation. */
bool sigma_fmt_write_rune(sigma_writer writer, sigma_rune value,
                          const sigma_fmt_spec *spec) {
  char encoded[4];
  usize len;
  sigma_fmt_spec string_spec = *spec;

  if (!sigma_rune_is_valid(value))
    return false;
  if (spec->presentation != SIGMA_FMT_PRESENT_DEFAULT &&
      spec->presentation != SIGMA_FMT_PRESENT_DEBUG)
    return false;
  if (value.value <= 0x7f) {
    encoded[0] = (char)value.value;
    len = 1;
  } else if (value.value <= 0x7ff) {
    encoded[0] = (char)(0xc0 | (value.value >> 6));
    encoded[1] = (char)(0x80 | (value.value & 0x3f));
    len = 2;
  } else if (value.value <= 0xffff) {
    encoded[0] = (char)(0xe0 | (value.value >> 12));
    encoded[1] = (char)(0x80 | ((value.value >> 6) & 0x3f));
    encoded[2] = (char)(0x80 | (value.value & 0x3f));
    len = 3;
  } else {
    encoded[0] = (char)(0xf0 | (value.value >> 18));
    encoded[1] = (char)(0x80 | ((value.value >> 12) & 0x3f));
    encoded[2] = (char)(0x80 | ((value.value >> 6) & 0x3f));
    encoded[3] = (char)(0x80 | (value.value & 0x3f));
    len = 4;
  }
  string_spec.presentation = SIGMA_FMT_PRESENT_DEFAULT;
  return sigma_fmt_write_str(writer, (str_t){.items = encoded, .len = len},
                             &string_spec);
}

/** Performs the sigma fmt write ptr operation. */
bool sigma_fmt_write_ptr(sigma_writer writer, const void *value,
                         const sigma_fmt_spec *spec) {
  sigma_fmt_spec integer_spec = *spec;

  if (spec->presentation != SIGMA_FMT_PRESENT_DEFAULT &&
      spec->presentation != SIGMA_FMT_PRESENT_POINTER)
    return false;
  integer_spec.presentation = SIGMA_FMT_PRESENT_HEX_LOWER;
  integer_spec.alternate = true;
  return sigma_fmt_write_u64(writer, (u64)(uptr)value, &integer_spec);
}
