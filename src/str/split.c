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
sigma_optional_str str_split_scalar_next(str_split_scalar_t *it) {
  if (it->finished)
    return NONE(sigma_optional_str);

  sigma_offset pos = str_find_byte(it->remaining, it->delimiter);

  if (!pos.some) {
    str_t field = it->remaining;
    it->remaining = (str_t){0};
    it->finished = true;
    return SOME(sigma_optional_str, field);
  }
  str_t field = str_sub(it->remaining, 0, pos.value);

  it->remaining = str_sub(it->remaining, pos.value + 1, it->remaining.len);

  return SOME(sigma_optional_str, field);
}
