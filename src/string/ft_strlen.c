#include <sigma/str.h>

#include "../internal/cpu.h"

#include <emmintrin.h>
#include <immintrin.h>

typedef usize (*strlen_fn)(const char *);

static usize strlen_scalar(const char *text) {
  const char *end = text;
  while (*end != '\0')
    end++;
  return (usize)(end - text);
}

static usize strlen_sse2(const char *text) {
  const char *cursor = text;
  while (((uptr)cursor & 15) != 0) {
    if (*cursor == '\0')
      return (usize)(cursor - text);
    cursor++;
  }
  const __m128i zero = _mm_setzero_si128();
  for (;;) {
    __m128i chunk = _mm_load_si128((const __m128i *)cursor);
    u32 mask = (u32)_mm_movemask_epi8(_mm_cmpeq_epi8(chunk, zero));
    if (mask != 0)
      return (usize)(cursor - text) + (usize)__builtin_ctz(mask);
    cursor += 16;
  }
}

[[gnu::target("avx2")]]
static usize strlen_avx2(const char *text) {
  const char *cursor = text;
  while (((uptr)cursor & 31) != 0) {
    if (*cursor == '\0')
      return (usize)(cursor - text);
    cursor++;
  }
  const __m256i zero = _mm256_setzero_si256();
  for (;;) {
    __m256i chunk = _mm256_load_si256((const __m256i *)cursor);
    u32 mask = (u32)_mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, zero));
    if (mask != 0) {
      usize result = (usize)(cursor - text) + (usize)__builtin_ctz(mask);
      _mm256_zeroupper();
      return result;
    }
    cursor += 32;
  }
}

static strlen_fn select_strlen(void) {
  switch (sigma_cpu_simd_level()) {
  case sigma_simd_avx2:
    return strlen_avx2;
  case sigma_simd_sse2:
    return strlen_sse2;
  case sigma_simd_scalar:
    return strlen_scalar;
  }
}

/* sigma:begin
name: libft.strlen
provides: str.len
deps:
externals:
kind: function
*/
usize ft_strlen(const char *text) {
  static strlen_fn implementation;
  if (implementation == nullptr)
    implementation = select_strlen();
  return implementation(text);
}
/* sigma:end */
