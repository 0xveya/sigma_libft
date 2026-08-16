/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:06:16 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/24 19:25:17 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.string.ft_strncmp
provides: libft.string.ft_strncmp
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

int ft_strncmp(const char s1[], const char s2[], size_t n) {
  while ((*s1 || *s2) && n--)
    if (*s1++ != *s2++)
      return ((unsigned char)*--s1 - (unsigned char)*--s2);
  return (0);
}

/* sigma:end */
