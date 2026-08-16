/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   print_hex.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/04/26 18:13:39 by sfurst           #+#    #+#              */
/*   Updated: 2026/04/26 18:51:52 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.io.printf.conversion.print_hex
provides: libft.io.printf.conversion.print_hex
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

static int ft_print_hex(t_writer *w, uint32_t n, t_format *f,
                        const char *alphabet) {
  char buf[16];
  char *begin;
  char *end;

  if (ft_format_has(f, FMT_HASH) && n != 0) {
    ft_writer_char(w, '0');
    ft_writer_char(w, alphabet[16]);
  }
  end = buf + sizeof(buf);
  begin = ft_u64_pow2(end, n, 4, alphabet);
  ft_writer_write(w, begin, (size_t)(end - begin));
  return (!w->error);
}

int ft_print_hex_low_fmt(t_writer *w, unsigned int n, t_format *f) {
  return (ft_print_hex(w, (uint32_t)n, f, "0123456789abcdefx"));
}

int ft_print_hex_up_fmt(t_writer *w, unsigned int n, t_format *f) {
  return (ft_print_hex(w, (uint32_t)n, f, "0123456789ABCDEFX"));
}
/* sigma:end */
