#include <sigma/bytes.h>
#include <sigma/qol.h>

/** Performs the bytes is empty operation. */
bool bytes_is_empty(bytes_t bytes) { return bytes.len == 0; }

/** Performs the bytes sub operation. */
bytes_t bytes_sub(bytes_t bytes, usize start, usize len) {
  if (start > bytes.len)
    start = bytes.len;

  usize remaining = bytes.len - start;

  if (len > remaining)
    len = remaining;

  return (bytes_t){
      .items = bytes.items != NULL ? bytes.items + start : NULL,
      .len = len,
  };
}

/** Performs the bytes mut sub operation. */
bytes_mut_t bytes_mut_sub(bytes_mut_t bytes, usize start, usize len) {
  if (start > bytes.len)
    start = bytes.len;

  usize remaining = bytes.len - start;

  if (len > remaining)
    len = remaining;

  return (bytes_mut_t){
      .items = bytes.items != NULL ? bytes.items + start : NULL,
      .len = len,
  };
}
