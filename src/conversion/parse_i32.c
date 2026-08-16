#include <sigma/libft.h>

/* sigma:begin
name: libft.parse_i32
provides: int.parse.i32
deps:
externals:
kind: function
*/
sigma_parse_i32_result sigma_parse_i32(const char *text) {
  sigma_parse_i32_result result = {.tag = sigma_parse_i32_invalid};
  const char *digits = text;
  u64 magnitude = 0;
  u64 limit = INT32_MAX;

  if (text == nullptr || *text == '\0')
    return result;
  if (*digits == '+' || *digits == '-')
    digits++;
  if (*digits == '\0')
    return result;
  if (*text == '-')
    limit++;
  while (*digits != '\0') {
    if (*digits < '0' || *digits > '9')
      return result;
    u64 digit = (u64)(*digits - '0');
    if (magnitude > (limit - digit) / 10)
      return (sigma_parse_i32_result){.tag = sigma_parse_i32_overflow};
    magnitude = magnitude * 10 + digit;
    digits++;
  }
  result.tag = sigma_parse_i32_ok;
  result.value = *text == '-' ? (i32)-(i64)magnitude : (i32)magnitude;
  return result;
}
/* sigma:end */

/* sigma:begin
name: libft.atoi
provides: int.atoi
deps: int.parse.i32
externals:
kind: function
*/
int ft_atoi(const char *text) {
  sigma_parse_i32_result result = sigma_parse_i32(text);

  return result.tag == sigma_parse_i32_ok ? result.value : 0;
}
/* sigma:end */
