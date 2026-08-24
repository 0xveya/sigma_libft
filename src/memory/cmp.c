#include <sigma/mem.h>

#include "../internal/cpu.h"

#include <emmintrin.h>
#include <immintrin.h>

typedef i32 (*mem_cmp_fn)(const u8 *, const u8 *, usize);

static i32 mem_cmp_scalar(const u8 *left, const u8 *right, usize len) {
  for (usize i = 0; i < len; ++i) {
    if (left[i] != right[i])
      return (i32)left[i] - (i32)right[i];
  }

  return 0;
}

static i32 mem_cmp_sse2(const u8 *left, const u8 *right, usize len) {
  usize i = 0;

  while (len - i >= 64) {
    for (usize offset = 0; offset < 64; offset += 16) {
      const __m128i a = _mm_loadu_si128((const __m128i *)(left + i + offset));
      const __m128i b = _mm_loadu_si128((const __m128i *)(right + i + offset));
      const u32 equal = (u32)_mm_movemask_epi8(_mm_cmpeq_epi8(a, b));

      if (equal != 0xffff) {
        usize mismatch = (usize)__builtin_ctz((~equal) & 0xffff);
        return (i32)left[i + offset + mismatch] -
               (i32)right[i + offset + mismatch];
      }
    }

    i += 64;
  }

  while (len - i >= 16) {
    const __m128i a = _mm_loadu_si128((const __m128i *)(left + i));
    const __m128i b = _mm_loadu_si128((const __m128i *)(right + i));
    const u32 equal = (u32)_mm_movemask_epi8(_mm_cmpeq_epi8(a, b));

    if (equal != 0xffff) {
      usize offset = (usize)__builtin_ctz((~equal) & 0xffff);
      return (i32)left[i + offset] - (i32)right[i + offset];
    }

    i += 16;
  }

  if (i == len)
    return 0;

  return mem_cmp_scalar(left + i, right + i, len - i);
}

[[gnu::target("avx2")]]
static i32 mem_cmp_avx2(const u8 *left, const u8 *right, usize len) {
  usize i = 0;

  while (len - i >= 128) {
    for (usize offset = 0; offset < 128; offset += 32) {
      const __m256i a =
          _mm256_loadu_si256((const __m256i *)(left + i + offset));
      const __m256i b =
          _mm256_loadu_si256((const __m256i *)(right + i + offset));
      const u32 equal = (u32)_mm256_movemask_epi8(_mm256_cmpeq_epi8(a, b));

      if (equal != UINT32_MAX) {
        usize mismatch = (usize)__builtin_ctz(~equal);
        _mm256_zeroupper();
        return (i32)left[i + offset + mismatch] -
               (i32)right[i + offset + mismatch];
      }
    }

    i += 128;
  }

  while (len - i >= 32) {
    const __m256i a = _mm256_loadu_si256((const __m256i *)(left + i));
    const __m256i b = _mm256_loadu_si256((const __m256i *)(right + i));
    const u32 equal = (u32)_mm256_movemask_epi8(_mm256_cmpeq_epi8(a, b));

    if (equal != UINT32_MAX) {
      usize offset = (usize)__builtin_ctz(~equal);
      _mm256_zeroupper();
      return (i32)left[i + offset] - (i32)right[i + offset];
    }

    i += 32;
  }

  _mm256_zeroupper();
  if (i == len)
    return 0;

  return mem_cmp_scalar(left + i, right + i, len - i);
}

static mem_cmp_fn select_mem_cmp(void) {
  switch (sigma_cpu_simd_level()) {
  case sigma_simd_avx2:
    return mem_cmp_avx2;
  case sigma_simd_sse2:
    return mem_cmp_sse2;
  case sigma_simd_scalar:
    return mem_cmp_scalar;
  }
}

/* sigma:begin
name: libft.memory.cmp.mem_cmp
provides: mem.cmp
deps:
externals: cpuid
kind: function
*/
i32 mem_cmp(const void *a, const void *b, usize len) {
  static mem_cmp_fn implementation;

  if (implementation == nullptr)
    implementation = select_mem_cmp();

  return implementation(a, b, len);
}
/* sigma:end */
