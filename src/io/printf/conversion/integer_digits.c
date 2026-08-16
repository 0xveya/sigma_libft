/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   integer_digits.c                                  :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/15 23:50:21 by sfurst           #+#    #+#              */
/*   Updated: 2026/08/15 23:56:45 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.io.printf.conversion.integer_digits
provides: libft.io.printf.conversion.integer_digits
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

static const char *ft_digits100(void) {
  return ("00010203040506070809"
          "10111213141516171819"
          "20212223242526272829"
          "30313233343536373839"
          "40414243444546474849"
          "50515253545556575859"
          "60616263646566676869"
          "70717273747576777879"
          "80818283848586878889"
          "90919293949596979899");
}

char *ft_u64_dec(char *end, uint64_t n) {
  const char *digits100;
  uint32_t pair;

  digits100 = ft_digits100();
  if (n == 0)
    *--end = '0';
  while (n >= 100) {
    pair = (uint32_t)(n % 100);
    n /= 100;
    end -= 2;
    end[0] = digits100[pair * 2];
    end[1] = digits100[pair * 2 + 1];
  }
  if (n > 0 && n < 10)
    *--end = (char)('0' + n);
  else if (n >= 10) {
    pair = (uint32_t)n;
    end -= 2;
    end[0] = digits100[pair * 2];
    end[1] = digits100[pair * 2 + 1];
  }
  return (end);
}

char *ft_u64_pow2(char *end, uint64_t n, uint32_t shift, const char *digits) {
  uint64_t mask;

  mask = (UINT64_C(1) << shift) - 1;
  if (n == 0)
    *--end = '0';
  while (n != 0) {
    *--end = digits[n & mask];
    n >>= shift;
  }
  return (end);
}

char *ft_u64_base(char *end, uint64_t n, uint32_t base, const char *digits) {
  if (n == 0)
    *--end = '0';
  while (n != 0) {
    *--end = digits[n % base];
    n /= base;
  }
  return (end);
}
/* sigma:end */
