/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:13:26 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/24 19:12:47 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.conversion.ft_itoa
provides: libft.conversion.ft_itoa
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

static int ft_numlen(long n) {
  int len;

  len = 0;
  if (n <= 0)
    len = 1;
  while (n) {
    len++;
    n /= 10;
  }
  return (len);
}

char *ft_itoa(int n) {
  char *str;
  long nb;
  int len;

  nb = n;
  len = ft_numlen(nb);
  str = malloc((usize)len + 1);
  if (!str)
    return (NULL);
  str[len] = '\0';
  if (nb == 0)
    str[0] = '0';
  if (nb < 0) {
    str[0] = '-';
    nb = -nb;
  }
  while (nb > 0) {
    str[--len] = (nb % 10) + '0';
    nb /= 10;
  }
  return (str);
}
/* sigma:end */
