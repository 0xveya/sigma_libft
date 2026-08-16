#pragma once

#include <sigma/attributes.h>
#include <sigma/qol.h>

typedef enum sigma_parse_i32_tag {
  sigma_parse_i32_ok,
  sigma_parse_i32_invalid,
  sigma_parse_i32_overflow,
} sigma_parse_i32_tag;

typedef struct sigma_parse_i32_result {
  sigma_parse_i32_tag tag;
  i32 value;
} sigma_parse_i32_result;

SIGMA_NODISCARD sigma_parse_i32_result sigma_parse_i32(const char *text);
int ft_atoi(const char *text);
SIGMA_NODISCARD char *ft_itoa(int value);
