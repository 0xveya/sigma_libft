#pragma once

#include <cpuid.h>
#include <stdbool.h>

static inline bool sigma_cpu_has_avx2(void) {
  unsigned int eax;
  unsigned int ebx;
  unsigned int ecx;
  unsigned int edx;

  if (__get_cpuid_max(0, nullptr) < 7)
    return false;
  __cpuid(1, eax, ebx, ecx, edx);
  if ((ecx & bit_AVX) == 0 || (ecx & bit_OSXSAVE) == 0)
    return false;
  unsigned int xcr0_low;
  unsigned int xcr0_high;
  __asm__ volatile("xgetbv" : "=a"(xcr0_low), "=d"(xcr0_high) : "c"(0));
  (void)xcr0_high;
  if ((xcr0_low & 0x6) != 0x6)
    return false;
  __cpuid_count(7, 0, eax, ebx, ecx, edx);
  return (ebx & bit_AVX2) != 0;
}
