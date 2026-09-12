#include <sigma/ascii.h>
#include <sigma/qol.h>
#include <sigma/str.h>

/** Performs the str trim start ascii operation. */
str_t str_trim_start_ascii(str_t s) {
  usize start = 0;

  while (start < s.len && ascii_is_space((u8)s.items[start]))
    start++;

  return str_sub(s, start, s.len - start);
}

/** Performs the str trim end ascii operation. */
str_t str_trim_end_ascii(str_t s) {
  usize end = s.len;

  while (end > 0 && ascii_is_space((u8)s.items[end - 1]))
    end--;

  return str_sub(s, 0, end);
}

/** Performs the str trim ascii operation. */
str_t str_trim_ascii(str_t s) {
  return str_trim_end_ascii(str_trim_start_ascii(s));
}
