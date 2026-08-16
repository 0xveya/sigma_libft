#pragma once

#if defined(__GNUC__) || defined(__clang__)
#define SIGMA_NODISCARD __attribute__((warn_unused_result))
#define SIGMA_PRINTF(format_index, args_index)                                 \
  __attribute__((format(printf, format_index, args_index)))
#else
#define SIGMA_NODISCARD
#define SIGMA_PRINTF(format_index, args_index)
#endif
