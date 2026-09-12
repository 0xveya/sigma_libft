#include <sigma/mem.h>

#include "../internal/cpu.h"

#include <emmintrin.h>
#include <immintrin.h>

typedef usize (*mem_find_fn)(const u8 *, usize, u8);

static usize mem_find_scalar(const u8 *bytes, usize len, u8 value) {
  for (usize i = 0; i < len; ++i) {
    if (bytes[i] == value)
      return i;
  }

  return SIGMA_NPOS;
}

static usize mem_find_sse2(const u8 *bytes, usize len, u8 value) {
  const __m128i needle = _mm_set1_epi8((char)value);
  usize i = 0;

  while (len - i >= 64) {
    for (usize offset = 0; offset < 64; offset += 16) {
      const __m128i chunk =
          _mm_loadu_si128((const __m128i *)(bytes + i + offset));
      const u32 mask = (u32)_mm_movemask_epi8(_mm_cmpeq_epi8(chunk, needle));

      if (mask != 0)
        return i + offset + (usize)__builtin_ctz(mask);
    }

    i += 64;
  }

  while (len - i >= 16) {
    const __m128i chunk = _mm_loadu_si128((const __m128i *)(bytes + i));
    const u32 mask = (u32)_mm_movemask_epi8(_mm_cmpeq_epi8(chunk, needle));

    if (mask != 0)
      return i + (usize)__builtin_ctz(mask);

    i += 16;
  }

  if (i == len)
    return SIGMA_NPOS;

  usize tail = mem_find_scalar(bytes + i, len - i, value);
  return tail == SIGMA_NPOS ? SIGMA_NPOS : i + tail;
}

[[gnu::target("avx2")]]
static usize mem_find_avx2(const u8 *bytes, usize len, u8 value) {
  const __m256i needle = _mm256_set1_epi8((char)value);
  usize i = 0;

  while (len - i >= 128) {
    for (usize offset = 0; offset < 128; offset += 32) {
      const __m256i chunk =
          _mm256_loadu_si256((const __m256i *)(bytes + i + offset));
      const u32 mask =
          (u32)_mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, needle));

      if (mask != 0) {
        _mm256_zeroupper();
        return i + offset + (usize)__builtin_ctz(mask);
      }
    }

    i += 128;
  }

  while (len - i >= 32) {
    const __m256i chunk = _mm256_loadu_si256((const __m256i *)(bytes + i));
    const u32 mask =
        (u32)_mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, needle));

    if (mask != 0) {
      _mm256_zeroupper();
      return i + (usize)__builtin_ctz(mask);
    }

    i += 32;
  }

  _mm256_zeroupper();
  if (i == len)
    return SIGMA_NPOS;

  usize tail = mem_find_scalar(bytes + i, len - i, value);
  return tail == SIGMA_NPOS ? SIGMA_NPOS : i + tail;
}

static mem_find_fn select_mem_find(void) {
  switch (sigma_cpu_simd_level()) {
  case sigma_simd_avx2:
    return mem_find_avx2;
  case sigma_simd_sse2:
    return mem_find_sse2;
  case sigma_simd_scalar:
    return mem_find_scalar;
  }
}

/** Performs the mem find operation. */
usize mem_find(const void *ptr, usize len, u8 value) {
  static mem_find_fn implementation;

  if (implementation == nullptr)
    implementation = select_mem_find();

  return implementation(ptr, len, value);
}
