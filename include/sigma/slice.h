#pragma once

#include <sigma/qol.h>

#define SIGMA_SLICE(T, name)                                                   \
  typedef struct name {                                                        \
    usize len;                                                                 \
    T *items;                                                                  \
  } name
