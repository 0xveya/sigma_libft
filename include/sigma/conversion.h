#pragma once

#include <sigma/attributes.h>
#include <sigma/qol.h>

typedef enum sigma_parse_i32_error {
  sigma_parse_i32_invalid,
  sigma_parse_i32_overflow,
} sigma_parse_i32_error;

RESULT(sigma_parse_i32_result, i32, sigma_parse_i32_error);

/* Parses a complete decimal C string into an i32 with explicit errors. */
SIGMA_NODISCARD sigma_parse_i32_result sigma_parse_i32(const char *text);
/* Parses the initial decimal integer using libft-compatible semantics. */
int ft_atoi(const char *text);
/* Allocates a decimal C string for value, or returns NULL on failure. */
SIGMA_NODISCARD char *ft_itoa(int value);
