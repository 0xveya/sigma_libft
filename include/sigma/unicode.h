#pragma once

#include <sigma/rune.h>

#define SIGMA_UNICODE_VERSION "17.0.0"

/* Returns true when rune has the Unicode Alphabetic property. */
bool sigma_rune_is_alpha(sigma_rune rune);
/* Returns true when rune is alphabetic or a decimal digit. */
bool sigma_rune_is_alnum(sigma_rune rune);
/* Returns true when rune is a Unicode control character. */
bool sigma_rune_is_cntrl(sigma_rune rune);
/* Returns true when rune has the Unicode Decimal_Number category. */
bool sigma_rune_is_digit(sigma_rune rune);
/* Returns true when rune has the Unicode Lowercase property. */
bool sigma_rune_is_lower(sigma_rune rune);
/* Returns true when rune is printable as a standalone scalar. */
bool sigma_rune_is_print(sigma_rune rune);
/* Returns true when rune has the Unicode White_Space property. */
bool sigma_rune_is_space(sigma_rune rune);
/* Returns true when rune has the Unicode Uppercase property. */
bool sigma_rune_is_upper(sigma_rune rune);
/* Returns true when rune has the Unicode Hex_Digit property. */
bool sigma_rune_is_xdigit(sigma_rune rune);

/* Applies the Unicode simple lowercase mapping. */
sigma_rune sigma_rune_to_lower(sigma_rune rune);
/* Applies the Unicode simple uppercase mapping. */
sigma_rune sigma_rune_to_upper(sigma_rune rune);
