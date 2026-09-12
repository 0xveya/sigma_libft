#include <sigma/mem.h>

/** Performs the mem copy operation. */
void *mem_copy(void *dst, const void *src, usize len) {
  return ft_memcpy(dst, src, len);
}

/** Performs the sigma memcpy operation. */
void *sigma_memcpy(void *dst, const void *src, usize len) {
  return mem_copy(dst, src, len);
}
