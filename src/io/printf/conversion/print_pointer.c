/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   print_pointer.c                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/04/26 18:14:56 by sfurst           #+#    #+#              */
/*   Updated: 2026/08/15 23:59:50 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.io.printf.conversion.print_pointer
provides: libft.io.printf.conversion.print_pointer
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

int ft_print_ptr_fmt(t_writer *w, void *ptr, t_format *f) {
  char buf[2 * sizeof(uintptr_t)];
  char *begin;
  char *end;

  (void)f;
  if (!ptr)
    return (ft_writer_write(w, "(nil)", 5));
  ft_writer_write(w, "0x", 2);
  end = buf + sizeof(buf);
  begin = ft_u64_pow2(end, (uint64_t)(uintptr_t)ptr, 4, "0123456789abcdef");
  ft_writer_write(w, begin, (size_t)(end - begin));
  return (!w->error);
}
/* sigma:end */
