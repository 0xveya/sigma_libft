#pragma once

#include <sigma/attributes.h>
#include <sigma/qol.h>
#include <stdbool.h>

#define SIGMA_SLICE(T, name)                                                   \
  typedef struct name {                                                        \
    usize len;                                                                 \
    T *items;                                                                  \
  } name

SIGMA_SLICE(char, str_t);

SIGMA_NODISCARD str_t sigma_str_from_cstr(const char *text);
bool sigma_str_eq(str_t left, str_t right);
