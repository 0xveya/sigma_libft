#include <sigma/str.h>

/** Performs the str split scalar operation. */
str_split_scalar_t str_split_scalar(str_t s, u8 delimiter) {
  return (str_split_scalar_t){
      .remaining = s,
      .delimiter = delimiter,
      .finished = false,
  };
}

/** Performs the str split scalar next operation. */
bool str_split_scalar_next(str_split_scalar_t *it, str_t *out) {
  if (it->finished)
    return false;

  usize pos = str_find_byte(it->remaining, it->delimiter);

  if (pos == SIGMA_NPOS) {
    *out = it->remaining;
    it->remaining = (str_t){0};
    it->finished = true;
    return true;
  }
  *out = str_sub(it->remaining, 0, pos);

  it->remaining = str_sub(it->remaining, pos + 1, it->remaining.len);

  return true;
}
