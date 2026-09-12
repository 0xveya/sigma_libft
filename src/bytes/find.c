#include <sigma/bytes.h>
#include <sigma/mem.h>

/** Performs the bytes find operation. */
usize bytes_find(bytes_t bytes, u8 value) {
  return mem_find(bytes.items, bytes.len, value);
}

/** Performs the bytes rfind operation. */
usize bytes_rfind(bytes_t bytes, u8 value) {
  for (usize i = bytes.len; i > 0; --i) {
    if (bytes.items[i - 1] == value)
      return i - 1;
  }

  return SIGMA_NPOS;
}
