#include <sigma/mem.h>

#include "../internal/cpu.h"

#include <emmintrin.h>
#include <immintrin.h>

typedef void (*memset_fn)(u8 *, u8, usize);

static void memset_scalar(u8 *dst, u8 byte, usize size) {
  while (size-- != 0)
    *dst++ = byte;
}

static void memset_sse2(u8 *dst, u8 byte, usize size) {
  __m128i value = _mm_set1_epi8((char)byte);
  while (size >= 64) {
    _mm_storeu_si128((__m128i *)(dst + 0), value);
    _mm_storeu_si128((__m128i *)(dst + 16), value);
    _mm_storeu_si128((__m128i *)(dst + 32), value);
    _mm_storeu_si128((__m128i *)(dst + 48), value);
    dst += 64;
    size -= 64;
  }
  while (size >= 16) {
    _mm_storeu_si128((__m128i *)dst, value);
    dst += 16;
    size -= 16;
  }
  while (size-- != 0)
    *dst++ = byte;
}

[[gnu::target("avx2")]]
static void memset_avx2(u8 *dst, u8 byte, usize size) {
  __m256i value = _mm256_set1_epi8((char)byte);
  while (size >= 128) {
    _mm256_storeu_si256((__m256i *)(dst + 0), value);
    _mm256_storeu_si256((__m256i *)(dst + 32), value);
    _mm256_storeu_si256((__m256i *)(dst + 64), value);
    _mm256_storeu_si256((__m256i *)(dst + 96), value);
    dst += 128;
    size -= 128;
  }
  while (size >= 32) {
    _mm256_storeu_si256((__m256i *)dst, value);
    dst += 32;
    size -= 32;
  }
  while (size-- != 0)
    *dst++ = byte;
  _mm256_zeroupper();
}

static memset_fn select_memset(void) {
  switch (sigma_cpu_simd_level()) {
  case sigma_simd_avx2:
    return memset_avx2;
  case sigma_simd_sse2:
    return memset_sse2;
  case sigma_simd_scalar:
    return memset_scalar;
  }
}

/** Performs the ft memset operation. */
void *ft_memset(void *ptr, int byte, usize size) {
  static memset_fn implementation;
  if (implementation == nullptr)
    implementation = select_memset();
  implementation(ptr, (u8)byte, size);
  return ptr;
}
