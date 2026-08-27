#include "format_internal.h"

[[gnu::always_inline]] static inline bool sigma_fmt_is_digit(char c) {
  return c >= '0' && c <= '9';
}

[[gnu::always_inline]] static inline bool
sigma_fmt_parse_align(char c, sigma_fmt_align *align) {
  switch (c) {
  case '<':
    *align = SIGMA_FMT_ALIGN_LEFT;
    return true;
  case '>':
    *align = SIGMA_FMT_ALIGN_RIGHT;
    return true;
  case '^':
    *align = SIGMA_FMT_ALIGN_CENTER;
    return true;
  default:
    return false;
  }
}

[[gnu::always_inline]] static inline bool
sigma_fmt_parse_presentation(char c, sigma_fmt_presentation *presentation) {
  switch (c) {
  case 'd':
    *presentation = SIGMA_FMT_PRESENT_DEC;
    return true;
  case 'x':
    *presentation = SIGMA_FMT_PRESENT_HEX_LOWER;
    return true;
  case 'X':
    *presentation = SIGMA_FMT_PRESENT_HEX_UPPER;
    return true;
  case 'b':
    *presentation = SIGMA_FMT_PRESENT_BINARY;
    return true;
  case 'o':
    *presentation = SIGMA_FMT_PRESENT_OCTAL;
    return true;
  case 'p':
    *presentation = SIGMA_FMT_PRESENT_POINTER;
    return true;
  case '?':
    *presentation = SIGMA_FMT_PRESENT_DEBUG;
    return true;
  default:
    return false;
  }
}

static bool sigma_fmt_parse_number(str_t input, usize *pos, usize *out) {
  if (*pos >= input.len || !sigma_fmt_is_digit(input.items[*pos]))
    return false;

  usize value = 0;

  while (*pos < input.len && sigma_fmt_is_digit(input.items[*pos])) {
    usize digit = (usize)(input.items[*pos] - '0');

    value = value * 10 + digit;
    (*pos)++;
  }

  *out = value;
  return true;
}

/* sigma:begin
name: libft.io.printf.format.parse.sigma_fmt_parse_field
provides: libft.sigma_fmt_parse_field
deps:
externals:
kind: function
*/
sigma_fmt_parse_result sigma_fmt_parse_field(str_t input,
                                             sigma_fmt_field *field) {
  if (input.len == 0)
    return SIGMA_FMT_PARSE_END;

  if (input.items[0] != '{')
    return SIGMA_FMT_PARSE_ERROR;

  sigma_fmt_spec spec = {
      .width = 0,
      .precision = 0,
      .fill = ' ',
      .align = SIGMA_FMT_ALIGN_DEFAULT,
      .presentation = SIGMA_FMT_PRESENT_DEFAULT,
      .has_width = false,
      .has_precision = false,
      .alternate = false,
      .zero_pad = false,
      .sign_plus = false,
      .sign_space = false,
  };

  usize pos = 1;

  if (pos < input.len && input.items[pos] == '}') {
    *field = (sigma_fmt_field){.spec = spec, .consumed = pos + 1};
    return SIGMA_FMT_PARSE_OK;
  }

  if (pos >= input.len || input.items[pos] != ':')
    return SIGMA_FMT_PARSE_ERROR;
  pos++;

  if (pos + 1 < input.len) {
    sigma_fmt_align align;

    if (sigma_fmt_parse_align(input.items[pos + 1], &align)) {
      spec.fill = (u8)input.items[pos];
      spec.align = align;
      pos += 2;
    }
  }

  if (spec.align == SIGMA_FMT_ALIGN_DEFAULT && pos < input.len) {
    sigma_fmt_align align;

    if (sigma_fmt_parse_align(input.items[pos], &align)) {
      spec.align = align;
      pos++;
    }
  }

  if (pos < input.len) {
    if (input.items[pos] == '+') {
      spec.sign_plus = true;
      pos++;
    } else if (input.items[pos] == ' ') {
      spec.sign_space = true;
      pos++;
    }
  }

  if (pos < input.len && input.items[pos] == '#') {
    spec.alternate = true;
    pos++;
  }

  if (pos < input.len && input.items[pos] == '0') {
    spec.zero_pad = true;
    pos++;
  }

  if (pos < input.len && sigma_fmt_is_digit(input.items[pos])) {
    spec.has_width = true;
    if (!sigma_fmt_parse_number(input, &pos, &spec.width))
      return SIGMA_FMT_PARSE_ERROR;
  }

  if (pos < input.len && input.items[pos] == '.') {
    pos++;
    spec.has_precision = true;
    if (!sigma_fmt_parse_number(input, &pos, &spec.precision))
      return SIGMA_FMT_PARSE_ERROR;
  }

  if (pos < input.len && input.items[pos] != '}') {
    if (!sigma_fmt_parse_presentation(input.items[pos], &spec.presentation))
      return SIGMA_FMT_PARSE_ERROR;
    pos++;
  }

  if (pos >= input.len || input.items[pos] != '}')
    return SIGMA_FMT_PARSE_ERROR;

  *field = (sigma_fmt_field){.spec = spec, .consumed = pos + 1};
  return SIGMA_FMT_PARSE_OK;
}
/* sigma:end */
