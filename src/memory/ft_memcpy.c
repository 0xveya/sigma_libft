#include <sigma/mem.h>

#include "../internal/cpu.h"

#include <emmintrin.h>
#include <immintrin.h>

typedef void (*memcpy_fn)(u8 *restrict, const u8 *restrict, usize);

static void memcpy_scalar(u8 *restrict dst, const u8 *restrict src,
                          usize size) {
  while (size-- != 0)
    *dst++ = *src++;
}

static void memcpy_sse2(u8 *restrict dst, const u8 *restrict src, usize size) {
  while (size >= 16) {
    _mm_storeu_si128((__m128i *)dst, _mm_loadu_si128((const __m128i *)src));
    dst += 16;
    src += 16;
    size -= 16;
  }
  while (size-- != 0)
    *dst++ = *src++;
}

[[gnu::target("avx2")]]
static void memcpy_avx2(u8 *restrict dst, const u8 *restrict src, usize size) {
  while (size >= 32) {
    _mm256_storeu_si256((__m256i *)dst,
                        _mm256_loadu_si256((const __m256i *)src));
    dst += 32;
    src += 32;
    size -= 32;
  }
  while (size-- != 0)
    *dst++ = *src++;
  _mm256_zeroupper();
}

static memcpy_fn select_memcpy(void) {
  switch (sigma_cpu_simd_level()) {
  case sigma_simd_avx2:
    return memcpy_avx2;
  case sigma_simd_sse2:
    return memcpy_sse2;
  case sigma_simd_scalar:
    return memcpy_scalar;
  }
}

/* sigma:begin
name: libft.memcpy
provides: mem.copy
deps:
externals:
kind: function
*/
void *ft_memcpy(void *restrict dst, const void *restrict src, usize size) {
  static memcpy_fn implementation;
  if (implementation == nullptr)
    implementation = select_memcpy();
  implementation(dst, src, size);
  return dst;
}
/* sigma:end */
