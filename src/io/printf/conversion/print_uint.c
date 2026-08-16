/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   print_uint.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/04/26 18:16:21 by sfurst           #+#    #+#              */
/*   Updated: 2026/04/26 18:39:05 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.io.printf.conversion.print_uint
provides: libft.io.printf.conversion.print_uint
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

int ft_print_uint_fmt(t_writer *w, unsigned int n, t_format *f) {
  char buf[32];
  char *begin;
  char *end;

  (void)f;
  end = buf + sizeof(buf);
  begin = ft_u64_dec(end, (uint64_t)n);
  return (ft_writer_write(w, begin, (size_t)(end - begin)));
}
/* sigma:end */
