#pragma once

#include <sigma/qol.h>

/* Defines a borrowed mutable slice type containing a pointer and length. */
#define SIGMA_SLICE(T, name)                                                   \
  typedef struct name {                                                        \
    usize len;                                                                 \
    T *items;                                                                  \
  } name
