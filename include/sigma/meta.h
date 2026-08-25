#pragma once

#define SIGMA_CAT_I(a, b) a##b
#define SIGMA_CAT(a, b) SIGMA_CAT_I(a, b)

#define SIGMA_STRINGIFY_I(x) #x
#define SIGMA_STRINGIFY(x) SIGMA_STRINGIFY_I(x)

#define SIGMA_EMPTY()

#define SIGMA_DEFER(id) id SIGMA_EMPTY()

#define SIGMA_OBSTRUCT(...) __VA_ARGS__ SIGMA_DEFER(SIGMA_EMPTY)()

#define SIGMA_EXPAND_1(...) __VA_ARGS__
#define SIGMA_EXPAND_2(...)                                                    \
  SIGMA_EXPAND_1(SIGMA_EXPAND_1(SIGMA_EXPAND_1(__VA_ARGS__)))
#define SIGMA_EXPAND_4(...)                                                    \
  SIGMA_EXPAND_2(SIGMA_EXPAND_2(SIGMA_EXPAND_2(__VA_ARGS__)))
#define SIGMA_EXPAND(...)                                                      \
  SIGMA_EXPAND_4(SIGMA_EXPAND_4(SIGMA_EXPAND_4(__VA_ARGS__)))

#define SIGMA_FOR_EACH(m, ...)                                                 \
  __VA_OPT__(SIGMA_EXPAND(SIGMA_FOR_EACH_I(m, __VA_ARGS__)))

#define SIGMA_FOR_EACH_I(m, x, ...)                                            \
  m(x) __VA_OPT__(SIGMA_OBSTRUCT(SIGMA_FOR_EACH_AGAIN)()(m, __VA_ARGS__))

#define SIGMA_FOR_EACH_AGAIN() SIGMA_FOR_EACH_I

#define SIGMA_REPEAT_1(F, C, N) F(C, N)

#define SIGMA_REPEAT_2(F, C, N)                                                \
  SIGMA_REPEAT_1(F, C, (N) + 0)                                                \
  SIGMA_REPEAT_1(F, C, (N) + 1)

#define SIGMA_REPEAT_4(F, C, N)                                                \
  SIGMA_REPEAT_2(F, C, (N) + 0)                                                \
  SIGMA_REPEAT_2(F, C, (N) + 2)

#define SIGMA_REPEAT_8(F, C, N)                                                \
  SIGMA_REPEAT_4(F, C, (N) + 0)                                                \
  SIGMA_REPEAT_4(F, C, (N) + 4)

#define SIGMA_REPEAT_16(F, C, N)                                               \
  SIGMA_REPEAT_8(F, C, (N) + 0)                                                \
  SIGMA_REPEAT_8(F, C, (N) + 8)

#define SIGMA_REPEAT_32(F, C, N)                                               \
  SIGMA_REPEAT_16(F, C, (N) + 0)                                               \
  SIGMA_REPEAT_16(F, C, (N) + 16)

#define SIGMA_REPEAT_64(F, C, N)                                               \
  SIGMA_REPEAT_32(F, C, (N) + 0)                                               \
  SIGMA_REPEAT_32(F, C, (N) + 32)

#define SIGMA_REPEAT_128(F, C, N)                                              \
  SIGMA_REPEAT_64(F, C, (N) + 0)                                               \
  SIGMA_REPEAT_64(F, C, (N) + 64)

#define SIGMA_REPEAT_256(F, C, N)                                              \
  SIGMA_REPEAT_128(F, C, (N) + 0)                                              \
  SIGMA_REPEAT_128(F, C, (N) + 128)
