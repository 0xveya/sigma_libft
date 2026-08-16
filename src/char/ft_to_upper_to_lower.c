/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_to_upper_to_lower.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:30:00 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/24 19:27:02 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.char.ft_to_upper_to_lower
provides: libft.char.ft_to_upper_to_lower
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

int ft_toupper(int c) {
  if (c >= 'a' && c <= 'z')
    return (c - 32);
  else
    return (c);
}

int ft_tolower(int c) {
  if (c >= 'A' && c <= 'Z')
    return (c + 32);
  else
    return (c);
}

/* sigma:end */
