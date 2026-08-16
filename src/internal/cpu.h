#pragma once

#include <cpuid.h>
#include <stdbool.h>

typedef enum sigma_simd_level {
  sigma_simd_scalar,
  sigma_simd_sse2,
  sigma_simd_avx2,
} sigma_simd_level;

static inline sigma_simd_level sigma_cpu_simd_level(void) {
#if defined(SIGMA_SIMD_FORCE_SCALAR)
  return sigma_simd_scalar;
#elif defined(SIGMA_SIMD_FORCE_SSE2)
  return sigma_simd_sse2;
#else
  unsigned int eax;
  unsigned int ebx;
  unsigned int ecx;
  unsigned int edx;

  if (__get_cpuid_max(0, nullptr) < 1)
    return sigma_simd_scalar;
  __cpuid(1, eax, ebx, ecx, edx);
  if ((edx & bit_SSE2) == 0)
    return sigma_simd_scalar;
  if ((ecx & bit_AVX) == 0 || (ecx & bit_OSXSAVE) == 0)
    return sigma_simd_sse2;
  unsigned int xcr0_low;
  unsigned int xcr0_high;
  __asm__ volatile("xgetbv" : "=a"(xcr0_low), "=d"(xcr0_high) : "c"(0));
  (void)xcr0_high;
  if ((xcr0_low & 0x6) != 0x6)
    return sigma_simd_sse2;
  if (__get_cpuid_max(0, nullptr) < 7)
    return sigma_simd_sse2;
  __cpuid_count(7, 0, eax, ebx, ecx, edx);
  return (ebx & bit_AVX2) != 0 ? sigma_simd_avx2 : sigma_simd_sse2;
#endif
}
