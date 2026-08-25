#pragma once

#include <sigma/qol.h>

/* Returns true when c is an ASCII letter or decimal digit. */
bool ascii_is_alnum(u8 c);
/* Returns true when c is an ASCII letter. */
bool ascii_is_alpha(u8 c);
/* Returns true when c is an ASCII decimal digit. */
bool ascii_is_digit(u8 c);
/* Returns true when c is a lowercase ASCII letter. */
bool ascii_is_lower(u8 c);
/* Returns true when c is ASCII whitespace. */
bool ascii_is_space(u8 c);
/* Returns true when c is an uppercase ASCII letter. */
bool ascii_is_upper(u8 c);

/* Converts an ASCII letter to lowercase and leaves other bytes unchanged. */
u8 ascii_to_lower(u8 c);
/* Converts an ASCII letter to uppercase and leaves other bytes unchanged. */
u8 ascii_to_upper(u8 c);
