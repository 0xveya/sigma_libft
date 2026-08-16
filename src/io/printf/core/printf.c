/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   printf.c                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/04/25 01:33:21 by sfurst           #+#    #+#              */
/*   Updated: 2026/04/26 18:30:24 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.io.printf.core.printf
provides: libft.io.printf.core.printf
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

static void ft_print_next(t_writer *w, const char *fmt, int *i, va_list args) {
  t_format f;

  if (fmt[*i] != '%') {
    ft_writer_char(w, fmt[*i]);
    (*i)++;
    return;
  }
  (*i)++;
  ft_format_init(&f);
  *i = ft_parse_format(fmt, *i, &f);
  ft_dispatch_print(w, &f, args);
}

int ft_printf(const char *fmt, ...) {
  t_writer w;
  va_list args;
  int i;

  if (!fmt)
    return (-1);
  w = (t_writer){0};
  i = 0;
  va_start(args, fmt);
  while (fmt[i] && !w.error)
    ft_print_next(&w, fmt, &i, args);
  va_end(args);
  if (!w.error)
    ft_writer_flush(&w);
  if (w.error)
    return (-1);
  return (w.total);
}
/* sigma:end */
