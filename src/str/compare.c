#include <sigma/str.h>

/** Performs the str eq operation. */
bool str_eq(str_t a, str_t b) { return bytes_eq(str_bytes(a), str_bytes(b)); }

/** Performs the str cmp operation. */
i32 str_cmp(str_t a, str_t b) { return bytes_cmp(str_bytes(a), str_bytes(b)); }

/** Performs the str starts with operation. */
bool str_starts_with(str_t s, str_t prefix) {
  if (prefix.len > s.len)
    return false;

  return str_eq(str_sub(s, 0, prefix.len), prefix);
}

/** Performs the str ends with operation. */
bool str_ends_with(str_t s, str_t suffix) {
  if (suffix.len > s.len)
    return false;

  return str_eq(str_sub(s, s.len - suffix.len, suffix.len), suffix);
}
