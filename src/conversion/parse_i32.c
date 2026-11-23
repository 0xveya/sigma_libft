#include <sigma/libft.h>

/** Performs the sigma parse i32 operation. */
sigma_parse_i32_result sigma_parse_i32(const char *text) {
  const char *digits = text;
  u64 magnitude = 0;
  u64 limit = INT32_MAX;

  if (text == nullptr || *text == '\0')
    return ERR(sigma_parse_i32_result, sigma_parse_i32_invalid);
  if (*digits == '+' || *digits == '-')
    digits++;
  if (*digits == '\0')
    return ERR(sigma_parse_i32_result, sigma_parse_i32_invalid);
  if (*text == '-')
    limit++;
  while (*digits != '\0') {
    if (*digits < '0' || *digits > '9')
      return ERR(sigma_parse_i32_result, sigma_parse_i32_invalid);
    u64 digit = (u64)(*digits - '0');
    if (magnitude > (limit - digit) / 10)
      return ERR(sigma_parse_i32_result, sigma_parse_i32_overflow);
    magnitude = magnitude * 10 + digit;
    digits++;
  }
  i32 value = *text == '-' ? (i32) - (i64)magnitude : (i32)magnitude;
  return OK(sigma_parse_i32_result, value);
}

/** Performs the ft atoi operation. */
int ft_atoi(const char *text) {
  sigma_parse_i32_result result = sigma_parse_i32(text);

  return result.ok ? result.value : 0;
}
