/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   print_percent.c                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/04/26 18:14:39 by sfurst           #+#    #+#              */
/*   Updated: 2026/04/26 18:34:12 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.io.printf.conversion.print_percent
provides: libft.io.printf.conversion.print_percent
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

int ft_print_percent_fmt(t_writer *w, t_format *f) {
  (void)f;
  return (ft_writer_char(w, '%'));
}
/* sigma:end */
