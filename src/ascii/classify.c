#include <sigma/ascii.h>
#include <sigma/qol.h>

bool ascii_is_space(u8 c) { return c == ' ' || (c >= '\t' && c <= '\r'); }

bool ascii_is_digit(u8 c) { return (u8)(c - '0') <= 9; }

bool ascii_is_alpha(u8 c) {
  c |= 0x20;

  return (u8)(c - 'a') <= 25;
}

u8 ascii_to_lower(u8 c) {
  if (ascii_is_alpha(c))
    return c | 0x20;

  return c;
}

u8 ascii_to_upper(u8 c) {
  if (ascii_is_alpha(c))
    return c & (u8)~0x20;

  return c;
}
