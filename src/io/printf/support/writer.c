/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   writer.c                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/15 23:32:36 by sfurst           #+#    #+#              */
/*   Updated: 2026/08/15 23:32:37 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.io.printf.support.writer
provides: libft.io.printf.support.writer
deps:
externals:
kind: function
*/
#include "../printf_internal.h"

int ft_writer_flush(t_writer *w) {
  ssize_t written;
  size_t off;

  off = 0;
  while (off < w->len) {
    written = write(STDOUT_FILENO, w->buf + off, w->len - off);
    if (written <= 0) {
      w->error = 1;
      return (0);
    }
    off += (size_t)written;
  }
  w->len = 0;
  return (1);
}

int ft_writer_write(t_writer *w, const char *s, size_t len) {
  size_t space;
  size_t n;

  while (len > 0 && !w->error) {
    space = sizeof(w->buf) - w->len;
    if (space == 0) {
      if (!ft_writer_flush(w))
        return (0);
      space = sizeof(w->buf);
    }
    n = len;
    if (n > space)
      n = space;
    ft_memcpy(w->buf + w->len, s, n);
    w->len += n;
    w->total += (int)n;
    s += n;
    len -= n;
  }
  return (!w->error);
}

int ft_writer_char(t_writer *w, char c) { return (ft_writer_write(w, &c, 1)); }

int ft_writer_repeat(t_writer *w, char c, size_t n) {
  while (n > 0) {
    if (!ft_writer_char(w, c))
      return (0);
    n--;
  }
  return (1);
}
/* sigma:end */
