#pragma once

#include <sigma/qol.h>

typedef struct {
  u32 value;
} sigma_rune;

static inline sigma_rune sigma_rune_from_u32(u32 value) {
  return (sigma_rune){.value = value};
}

static inline bool sigma_rune_is_valid(sigma_rune rune) {
  return rune.value <= 0x10FFFF &&
         !(rune.value >= 0xD800 && rune.value <= 0xDFFF);
}
