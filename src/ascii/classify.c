#include <sigma/ascii.h>
#include <sigma/meta.h>
#include <sigma/qol.h>

#include "ascii.def.h"

#define SIGMA_ASCII_RANGE(c, lo, hi, mask)                                     \
  | (((c) >= (lo) && (c) <= (hi)) ? (mask) : 0)
#define SIGMA_ASCII_ONE(c, value, mask) | (((c) == (value)) ? (mask) : 0)
#define SIGMA_ASCII_FLAGS_FOR(c)                                               \
  (0 SIGMA_ASCII_RULES(SIGMA_ASCII_RANGE, SIGMA_ASCII_ONE, c))
#define SIGMA_ASCII_LOWER_FOR(c)                                               \
  (((c) >= 'A' && (c) <= 'Z') ? ((c) + ('a' - 'A')) : (c))
#define SIGMA_ASCII_UPPER_FOR(c)                                               \
  (((c) >= 'a' && (c) <= 'z') ? ((c) - ('a' - 'A')) : (c))
#define SIGMA_ASCII_ENTRY(_, c)                                                \
  {                                                                            \
      .flags = SIGMA_ASCII_FLAGS_FOR(c),                                       \
      .lower = SIGMA_ASCII_LOWER_FOR(c),                                       \
      .upper = SIGMA_ASCII_UPPER_FOR(c),                                       \
  },

static const sigma_ascii_info_t sigma_ascii_table[128] = {
    SIGMA_REPEAT_128(SIGMA_ASCII_ENTRY, _, 0)};

#define SIGMA_DEFINE_ASCII_PRED(name, mask)                                    \
  bool ascii_is_##name(u8 c) {                                                 \
    return c < 128 && (sigma_ascii_table[c].flags & (mask)) != 0;              \
  }

SIGMA_ASCII_PREDICATES(SIGMA_DEFINE_ASCII_PRED)

/* Converts an ASCII byte using the generated lowercase mapping. */
u8 ascii_to_lower(u8 c) { return c < 128 ? sigma_ascii_table[c].lower : c; }

/* Converts an ASCII byte using the generated uppercase mapping. */
u8 ascii_to_upper(u8 c) { return c < 128 ? sigma_ascii_table[c].upper : c; }
