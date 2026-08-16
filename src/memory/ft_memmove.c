/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:19:58 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/24 19:17:44 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.memory.ft_memmove
provides: libft.memory.ft_memmove
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void *ft_memmove(void *dst, const void *src, size_t len) {
  unsigned char *dest_char;
  unsigned char *src_char;

  dest_char = (unsigned char *)dst;
  src_char = (unsigned char *)src;
  if (!dst && !src)
    return (dst);
  if (src < dst)
    while (len--)
      dest_char[len] = src_char[len];
  else
    while (len--)
      *dest_char++ = *src_char++;
  return (dst);
}

/* sigma:end */
