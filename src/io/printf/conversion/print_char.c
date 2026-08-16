/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   print_char.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/04/26 18:09:18 by sfurst           #+#    #+#              */
/*   Updated: 2026/04/26 18:10:10 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.io.printf.conversion.print_char
provides: libft.io.printf.conversion.print_char
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

int ft_print_char_fmt(t_writer *w, int c, t_format *f) {
  int pad;

  pad = f->width - 1;
  if (pad < 0)
    pad = 0;
  if (!ft_format_has(f, FMT_MINUS))
    ft_writer_repeat(w, ' ', (size_t)pad);
  ft_writer_char(w, (char)c);
  if (ft_format_has(f, FMT_MINUS))
    ft_writer_repeat(w, ' ', (size_t)pad);
  return (!w->error);
}
/* sigma:end */
