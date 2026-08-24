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
  while (size >= 64) {
    __m128i a = _mm_loadu_si128((const __m128i *)(src + 0));
    __m128i b = _mm_loadu_si128((const __m128i *)(src + 16));
    __m128i c = _mm_loadu_si128((const __m128i *)(src + 32));
    __m128i d = _mm_loadu_si128((const __m128i *)(src + 48));
    _mm_storeu_si128((__m128i *)(dst + 0), a);
    _mm_storeu_si128((__m128i *)(dst + 16), b);
    _mm_storeu_si128((__m128i *)(dst + 32), c);
    _mm_storeu_si128((__m128i *)(dst + 48), d);
    dst += 64;
    src += 64;
    size -= 64;
  }
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
  while (size >= 128) {
    __m256i a = _mm256_loadu_si256((const __m256i *)(src + 0));
    __m256i b = _mm256_loadu_si256((const __m256i *)(src + 32));
    __m256i c = _mm256_loadu_si256((const __m256i *)(src + 64));
    __m256i d = _mm256_loadu_si256((const __m256i *)(src + 96));
    _mm256_storeu_si256((__m256i *)(dst + 0), a);
    _mm256_storeu_si256((__m256i *)(dst + 32), b);
    _mm256_storeu_si256((__m256i *)(dst + 64), c);
    _mm256_storeu_si256((__m256i *)(dst + 96), d);
    dst += 128;
    src += 128;
    size -= 128;
  }
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
name: libft.memory.ft_memcpy.ft_memcpy
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
