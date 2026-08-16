/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:00:06 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/24 19:25:54 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.string.ft_strrchr
provides: libft.string.ft_strrchr
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

char *ft_strrchr(const char *s, int c) {
  const char *str;

  str = s;
  while (*s)
    s++;
  while (s >= str)
    if (*s-- == (char)c)
      return ((char *)s + 1);
  return (NULL);
}

/* sigma:end */
