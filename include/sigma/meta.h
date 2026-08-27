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

/*
 * Count 0..64 variadic arguments.
 *
 * C23 __VA_OPT__ handles the empty argument list.
 */
#define SIGMA_PP_NARGS(...)                                                    \
  SIGMA_PP_NARGS_IMPL(__VA_ARGS__ __VA_OPT__(, ) 64, 63, 62, 61, 60, 59, 58,   \
                      57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44,  \
                      43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30,  \
                      29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,  \
                      15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define SIGMA_PP_NARGS_IMPL(                                                   \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16,     \
    _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, \
    _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, \
    _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, \
    _62, _63, _64, N, ...)                                                     \
  N

/*
 * Apply M to every argument and separate expansions with commas.
 *
 * SIGMA_PP_MAP(M, a, b, c)
 *
 * expands to:
 *
 * M(a), M(b), M(c)
 */
#define SIGMA_PP_MAP(M, ...)                                                   \
  SIGMA_CAT(SIGMA_PP_MAP_,                                                     \
            SIGMA_PP_NARGS(__VA_ARGS__))(M __VA_OPT__(, ) __VA_ARGS__)

#define SIGMA_PP_MAP_0(M)

#define SIGMA_PP_MAP_1(M, a, ...) M(a)

#define SIGMA_PP_MAP_2(M, a, ...) M(a), SIGMA_PP_MAP_1(M, __VA_ARGS__)

#define SIGMA_PP_MAP_3(M, a, ...) M(a), SIGMA_PP_MAP_2(M, __VA_ARGS__)

#define SIGMA_PP_MAP_4(M, a, ...) M(a), SIGMA_PP_MAP_3(M, __VA_ARGS__)

#define SIGMA_PP_MAP_5(M, a, ...) M(a), SIGMA_PP_MAP_4(M, __VA_ARGS__)

#define SIGMA_PP_MAP_6(M, a, ...) M(a), SIGMA_PP_MAP_5(M, __VA_ARGS__)

#define SIGMA_PP_MAP_7(M, a, ...) M(a), SIGMA_PP_MAP_6(M, __VA_ARGS__)

#define SIGMA_PP_MAP_8(M, a, ...) M(a), SIGMA_PP_MAP_7(M, __VA_ARGS__)

#define SIGMA_PP_MAP_9(M, a, ...) M(a), SIGMA_PP_MAP_8(M, __VA_ARGS__)

#define SIGMA_PP_MAP_10(M, a, ...) M(a), SIGMA_PP_MAP_9(M, __VA_ARGS__)

#define SIGMA_PP_MAP_11(M, a, ...) M(a), SIGMA_PP_MAP_10(M, __VA_ARGS__)

#define SIGMA_PP_MAP_12(M, a, ...) M(a), SIGMA_PP_MAP_11(M, __VA_ARGS__)

#define SIGMA_PP_MAP_13(M, a, ...) M(a), SIGMA_PP_MAP_12(M, __VA_ARGS__)

#define SIGMA_PP_MAP_14(M, a, ...) M(a), SIGMA_PP_MAP_13(M, __VA_ARGS__)

#define SIGMA_PP_MAP_15(M, a, ...) M(a), SIGMA_PP_MAP_14(M, __VA_ARGS__)

#define SIGMA_PP_MAP_16(M, a, ...) M(a), SIGMA_PP_MAP_15(M, __VA_ARGS__)

#define SIGMA_PP_MAP_17(M, a, ...) M(a), SIGMA_PP_MAP_16(M, __VA_ARGS__)

#define SIGMA_PP_MAP_18(M, a, ...) M(a), SIGMA_PP_MAP_17(M, __VA_ARGS__)

#define SIGMA_PP_MAP_19(M, a, ...) M(a), SIGMA_PP_MAP_18(M, __VA_ARGS__)

#define SIGMA_PP_MAP_20(M, a, ...) M(a), SIGMA_PP_MAP_19(M, __VA_ARGS__)

#define SIGMA_PP_MAP_21(M, a, ...) M(a), SIGMA_PP_MAP_20(M, __VA_ARGS__)

#define SIGMA_PP_MAP_22(M, a, ...) M(a), SIGMA_PP_MAP_21(M, __VA_ARGS__)

#define SIGMA_PP_MAP_23(M, a, ...) M(a), SIGMA_PP_MAP_22(M, __VA_ARGS__)

#define SIGMA_PP_MAP_24(M, a, ...) M(a), SIGMA_PP_MAP_23(M, __VA_ARGS__)

#define SIGMA_PP_MAP_25(M, a, ...) M(a), SIGMA_PP_MAP_24(M, __VA_ARGS__)

#define SIGMA_PP_MAP_26(M, a, ...) M(a), SIGMA_PP_MAP_25(M, __VA_ARGS__)

#define SIGMA_PP_MAP_27(M, a, ...) M(a), SIGMA_PP_MAP_26(M, __VA_ARGS__)

#define SIGMA_PP_MAP_28(M, a, ...) M(a), SIGMA_PP_MAP_27(M, __VA_ARGS__)

#define SIGMA_PP_MAP_29(M, a, ...) M(a), SIGMA_PP_MAP_28(M, __VA_ARGS__)

#define SIGMA_PP_MAP_30(M, a, ...) M(a), SIGMA_PP_MAP_29(M, __VA_ARGS__)

#define SIGMA_PP_MAP_31(M, a, ...) M(a), SIGMA_PP_MAP_30(M, __VA_ARGS__)

#define SIGMA_PP_MAP_32(M, a, ...) M(a), SIGMA_PP_MAP_31(M, __VA_ARGS__)

#define SIGMA_PP_MAP_33(M, a, ...) M(a), SIGMA_PP_MAP_32(M, __VA_ARGS__)

#define SIGMA_PP_MAP_34(M, a, ...) M(a), SIGMA_PP_MAP_33(M, __VA_ARGS__)

#define SIGMA_PP_MAP_35(M, a, ...) M(a), SIGMA_PP_MAP_34(M, __VA_ARGS__)

#define SIGMA_PP_MAP_36(M, a, ...) M(a), SIGMA_PP_MAP_35(M, __VA_ARGS__)

#define SIGMA_PP_MAP_37(M, a, ...) M(a), SIGMA_PP_MAP_36(M, __VA_ARGS__)

#define SIGMA_PP_MAP_38(M, a, ...) M(a), SIGMA_PP_MAP_37(M, __VA_ARGS__)

#define SIGMA_PP_MAP_39(M, a, ...) M(a), SIGMA_PP_MAP_38(M, __VA_ARGS__)

#define SIGMA_PP_MAP_40(M, a, ...) M(a), SIGMA_PP_MAP_39(M, __VA_ARGS__)

#define SIGMA_PP_MAP_41(M, a, ...) M(a), SIGMA_PP_MAP_40(M, __VA_ARGS__)

#define SIGMA_PP_MAP_42(M, a, ...) M(a), SIGMA_PP_MAP_41(M, __VA_ARGS__)

#define SIGMA_PP_MAP_43(M, a, ...) M(a), SIGMA_PP_MAP_42(M, __VA_ARGS__)

#define SIGMA_PP_MAP_44(M, a, ...) M(a), SIGMA_PP_MAP_43(M, __VA_ARGS__)

#define SIGMA_PP_MAP_45(M, a, ...) M(a), SIGMA_PP_MAP_44(M, __VA_ARGS__)

#define SIGMA_PP_MAP_46(M, a, ...) M(a), SIGMA_PP_MAP_45(M, __VA_ARGS__)

#define SIGMA_PP_MAP_47(M, a, ...) M(a), SIGMA_PP_MAP_46(M, __VA_ARGS__)

#define SIGMA_PP_MAP_48(M, a, ...) M(a), SIGMA_PP_MAP_47(M, __VA_ARGS__)

#define SIGMA_PP_MAP_49(M, a, ...) M(a), SIGMA_PP_MAP_48(M, __VA_ARGS__)

#define SIGMA_PP_MAP_50(M, a, ...) M(a), SIGMA_PP_MAP_49(M, __VA_ARGS__)

#define SIGMA_PP_MAP_51(M, a, ...) M(a), SIGMA_PP_MAP_50(M, __VA_ARGS__)

#define SIGMA_PP_MAP_52(M, a, ...) M(a), SIGMA_PP_MAP_51(M, __VA_ARGS__)

#define SIGMA_PP_MAP_53(M, a, ...) M(a), SIGMA_PP_MAP_52(M, __VA_ARGS__)

#define SIGMA_PP_MAP_54(M, a, ...) M(a), SIGMA_PP_MAP_53(M, __VA_ARGS__)

#define SIGMA_PP_MAP_55(M, a, ...) M(a), SIGMA_PP_MAP_54(M, __VA_ARGS__)

#define SIGMA_PP_MAP_56(M, a, ...) M(a), SIGMA_PP_MAP_55(M, __VA_ARGS__)

#define SIGMA_PP_MAP_57(M, a, ...) M(a), SIGMA_PP_MAP_56(M, __VA_ARGS__)

#define SIGMA_PP_MAP_58(M, a, ...) M(a), SIGMA_PP_MAP_57(M, __VA_ARGS__)

#define SIGMA_PP_MAP_59(M, a, ...) M(a), SIGMA_PP_MAP_58(M, __VA_ARGS__)

#define SIGMA_PP_MAP_60(M, a, ...) M(a), SIGMA_PP_MAP_59(M, __VA_ARGS__)

#define SIGMA_PP_MAP_61(M, a, ...) M(a), SIGMA_PP_MAP_60(M, __VA_ARGS__)

#define SIGMA_PP_MAP_62(M, a, ...) M(a), SIGMA_PP_MAP_61(M, __VA_ARGS__)

#define SIGMA_PP_MAP_63(M, a, ...) M(a), SIGMA_PP_MAP_62(M, __VA_ARGS__)

#define SIGMA_PP_MAP_64(M, a, ...) M(a), SIGMA_PP_MAP_63(M, __VA_ARGS__)
