#include <sigma/diagnostic.h>
#include <sigma/unicode.h>

#include <stddef.h>

enum sigma_unicode_flag : u16 {
  SIGMA_UNICODE_ALPHA = 1u << 0,
  SIGMA_UNICODE_CNTRL = 1u << 1,
  SIGMA_UNICODE_DIGIT = 1u << 2,
  SIGMA_UNICODE_LOWER = 1u << 3,
  SIGMA_UNICODE_PRINT = 1u << 4,
  SIGMA_UNICODE_SPACE = 1u << 5,
  SIGMA_UNICODE_UPPER = 1u << 6,
  SIGMA_UNICODE_XDIGIT = 1u << 7,
};

enum {
  SIGMA_UNICODE_RECORD_SIZE = 18,
};

// clang-format off
static constexpr u8 sigma_unicode_data[] = {
#embed "unicode.bin"
};
// clang-format on

SIGMA_STATIC_ASSERT(
    sizeof(sigma_unicode_data) % SIGMA_UNICODE_RECORD_SIZE == 0,
    "SIGMA_E_UNICODE_DATABASE_SIZE",
    "generated Unicode database has a partial record", sigma_unicode_data,
    "regenerate unicode.bin with records of SIGMA_UNICODE_RECORD_SIZE bytes");

typedef struct {
  u32 first;
  u32 last;
  u16 flags;
  i32 lower_delta;
  i32 upper_delta;
} sigma_unicode_record_t;

/* Decodes one little-endian u16 from the embedded database. */
static u16 decode_u16(const u8 *bytes) {
  return (u16)((u16)bytes[0] | (u16)((u16)bytes[1] << 8));
}

/* Decodes one little-endian u32 from the embedded database. */
static u32 decode_u32(const u8 *bytes) {
  return (u32)bytes[0] | ((u32)bytes[1] << 8) | ((u32)bytes[2] << 16) |
         ((u32)bytes[3] << 24);
}

/* Decodes a generated record without relying on host padding or endianness. */
static sigma_unicode_record_t decode_record(usize index) {
  const u8 *bytes =
      &sigma_unicode_data[index * (usize)SIGMA_UNICODE_RECORD_SIZE];

  return (sigma_unicode_record_t){
      .first = decode_u32(bytes),
      .last = decode_u32(bytes + 4),
      .flags = decode_u16(bytes + 8),
      .lower_delta = (i32)decode_u32(bytes + 10),
      .upper_delta = (i32)decode_u32(bytes + 14),
  };
}

/* Finds the generated record containing rune or returns false for a gap. */
static bool find_record(sigma_rune rune, sigma_unicode_record_t *out) {
  if (!sigma_rune_is_valid(rune))
    return false;

  usize low = 0;
  usize high = sizeof(sigma_unicode_data) / SIGMA_UNICODE_RECORD_SIZE;

  while (low < high) {
    usize middle = low + (high - low) / 2;
    sigma_unicode_record_t record = decode_record(middle);

    if (rune.value < record.first)
      high = middle;
    else if (rune.value > record.last)
      low = middle + 1;
    else {
      *out = record;
      return true;
    }
  }

  return false;
}

/* Tests one generated Unicode property flag. */
static bool has_flag(sigma_rune rune, enum sigma_unicode_flag flag) {
  sigma_unicode_record_t record;
  return find_record(rune, &record) && (record.flags & flag) != 0;
}

#define SIGMA_UNICODE_PREDICATES(X)                                            \
  X(alnum, SIGMA_UNICODE_ALPHA | SIGMA_UNICODE_DIGIT)                          \
  X(alpha, SIGMA_UNICODE_ALPHA)                                                \
  X(cntrl, SIGMA_UNICODE_CNTRL)                                                \
  X(digit, SIGMA_UNICODE_DIGIT)                                                \
  X(lower, SIGMA_UNICODE_LOWER)                                                \
  X(print, SIGMA_UNICODE_PRINT)                                                \
  X(space, SIGMA_UNICODE_SPACE)                                                \
  X(upper, SIGMA_UNICODE_UPPER)                                                \
  X(xdigit, SIGMA_UNICODE_XDIGIT)

#define SIGMA_DEFINE_UNICODE_PREDICATE(name, flag)                             \
  bool sigma_rune_is_##name(sigma_rune rune) { return has_flag(rune, flag); }

SIGMA_UNICODE_PREDICATES(SIGMA_DEFINE_UNICODE_PREDICATE)

sigma_rune sigma_rune_to_lower(sigma_rune rune) {
  sigma_unicode_record_t record;
  if (!find_record(rune, &record))
    return rune;

  return sigma_rune_from_u32((u32)((i64)rune.value + record.lower_delta));
}

sigma_rune sigma_rune_to_upper(sigma_rune rune) {
  sigma_unicode_record_t record;
  if (!find_record(rune, &record))
    return rune;

  return sigma_rune_from_u32((u32)((i64)rune.value + record.upper_delta));
}
