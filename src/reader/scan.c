#include "reader_internal.h"

#include "../internal/cpu.h"

#include <emmintrin.h>
#include <immintrin.h>

typedef usize (*scan_fn)(const u8 *, usize);

static usize scan_scalar(const u8 *bytes, usize size) {
  for (usize index = 0; index < size; index++)
    if (bytes[index] == '\n')
      return index;
  return size;
}

static usize scan_sse2(const u8 *bytes, usize size) {
  const __m128i newline = _mm_set1_epi8('\n');
  usize index = 0;
  while (size - index >= 16) {
    __m128i chunk = _mm_loadu_si128((const __m128i *)(bytes + index));
    u32 mask = (u32)_mm_movemask_epi8(_mm_cmpeq_epi8(chunk, newline));
    if (mask != 0)
      return index + (usize)__builtin_ctz(mask);
    index += 16;
  }
  return index + scan_scalar(bytes + index, size - index);
}

[[gnu::target("avx2")]]
static usize scan_avx2(const u8 *bytes, usize size) {
  const __m256i newline = _mm256_set1_epi8('\n');
  usize index = 0;
  while (size - index >= 32) {
    __m256i chunk = _mm256_loadu_si256((const __m256i *)(bytes + index));
    u32 mask = (u32)_mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, newline));
    if (mask != 0) {
      _mm256_zeroupper();
      return index + (usize)__builtin_ctz(mask);
    }
    index += 32;
  }
  _mm256_zeroupper();
  return index + scan_scalar(bytes + index, size - index);
}

/* sigma:begin
name: libft.reader.scan.sigma_reader_find_newline
provides: io.reader.scan
deps:
externals:
kind: function
*/
usize sigma_reader_find_newline(const u8 *bytes, usize size) {
  static scan_fn implementation;
  if (implementation == nullptr) {
    switch (sigma_cpu_simd_level()) {
    case sigma_simd_avx2:
      implementation = scan_avx2;
      break;
    case sigma_simd_sse2:
      implementation = scan_sse2;
      break;
    case sigma_simd_scalar:
      implementation = scan_scalar;
      break;
    }
  }
  return implementation(bytes, size);
}
/* sigma:end */
