/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 12:34:33 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/24 19:10:25 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.memory.ft_calloc
provides: libft.memory.ft_calloc
deps:
externals:
kind: function
*/
#include <sigma/libft.h>
#include <stdint.h>

void *ft_calloc(size_t n, size_t size) {
  void *ptr;
  size_t total;

  if (n != 0 && size > SIZE_MAX / n)
    return (NULL);
  total = n * size;
  if (total == 0)
    total = 1;
  ptr = malloc(total);
  if (!ptr)
    return (NULL);
  return (ft_bzero(ptr, total), ptr);
}

/* sigma:end */
