/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   print_string.c                                    :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/04/26 18:16:02 by sfurst           #+#    #+#              */
/*   Updated: 2026/04/26 18:18:57 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.io.printf.conversion.print_string
provides: libft.io.printf.conversion.print_string
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

int ft_print_str_fmt(t_writer *w, const char *s, t_format *f) {
  int len;
  int pad;

  if (!s)
    s = "(null)";
  len = (int)ft_strlen(s);
  if (f->precision >= 0 && f->precision < len)
    len = f->precision;
  pad = f->width - len;
  if (pad < 0)
    pad = 0;
  if (!ft_format_has(f, FMT_MINUS))
    ft_writer_repeat(w, ' ', (size_t)pad);
  ft_writer_write(w, s, (size_t)len);
  if (ft_format_has(f, FMT_MINUS))
    ft_writer_repeat(w, ' ', (size_t)pad);
  return (!w->error);
}
/* sigma:end */
