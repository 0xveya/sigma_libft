#include <sigma/bytes.h>
#include <sigma/mem.h>

/** Performs the bytes eq operation. */
bool bytes_eq(bytes_t a, bytes_t b) {
  return a.len == b.len && mem_cmp(a.items, b.items, a.len) == 0;
}

/** Performs the bytes cmp operation. */
i32 bytes_cmp(bytes_t a, bytes_t b) {
  usize len = a.len < b.len ? a.len : b.len;

  i32 cmp = mem_cmp(a.items, b.items, len);

  if (cmp != 0)
    return cmp;

  if (a.len < b.len)
    return -1;

  if (a.len > b.len)
    return 1;

  return 0;
}
