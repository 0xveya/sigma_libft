#include <sigma/printf.h>

#define CONTRACT_WRAP(value) (value)
#define CONTRACT_ARGS_64                                                       \
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,   \
      22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,  \
      40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,  \
      58, 59, 60, 61, 62, 63, 64

SIGMA_STATIC_ASSERT(SIGMA_PP_NARGS() == 0, "SIGMA_TEST_NARGS_0",
                    "empty argument counting changed", SIGMA_PP_NARGS(),
                    "SIGMA_PP_NARGS must support zero arguments");
SIGMA_STATIC_ASSERT(SIGMA_PP_NARGS(CONTRACT_ARGS_64) == 64,
                    "SIGMA_TEST_NARGS_64", "64 argument counting changed",
                    SIGMA_PP_NARGS(CONTRACT_ARGS_64),
                    "SIGMA_PP_NARGS must support 64 arguments");
SIGMA_STATIC_ASSERT(SIGMA_FMT_TYPE_SUPPORTED(42), "SIGMA_TEST_FORMAT_INT",
                    "int disappeared from the formatting registry", int,
                    "restore the builtin int formatting association");
SIGMA_STATIC_ASSERT(SIGMA_FMT_TYPE_SUPPORTED(STR_LIT("contract")),
                    "SIGMA_TEST_FORMAT_STR",
                    "str_t disappeared from the formatting registry", str_t,
                    "restore the builtin str_t formatting association");

static const int contract_empty[] = {0 SIGMA_PP_MAP(CONTRACT_WRAP)};
static const int contract_four[] = {SIGMA_PP_MAP(CONTRACT_WRAP, 1, 2, 3, 4)};
static const int contract_map[] = {
    SIGMA_PP_MAP(CONTRACT_WRAP, CONTRACT_ARGS_64)};

SIGMA_STATIC_ASSERT(sizeof(contract_empty) / sizeof(*contract_empty) == 1,
                    "SIGMA_TEST_MAP_0", "empty mapping emitted an argument",
                    contract_empty,
                    "SIGMA_PP_MAP with no arguments must expand to nothing");
SIGMA_STATIC_ASSERT(sizeof(contract_four) / sizeof(*contract_four) == 4,
                    "SIGMA_TEST_MAP_4", "four argument mapping changed",
                    contract_four,
                    "SIGMA_PP_MAP must preserve argument order and count");
SIGMA_STATIC_ASSERT(sizeof(contract_map) / sizeof(*contract_map) == 64,
                    "SIGMA_TEST_MAP_64", "64 argument mapping changed",
                    contract_map,
                    "SIGMA_PP_MAP must emit one element for every argument");

bool sigma_test_meta(void) {
  return contract_four[0] == 1 && contract_four[3] == 4 &&
         contract_map[0] == 1 && contract_map[63] == 64;
}
