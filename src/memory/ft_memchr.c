/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:09:39 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/24 19:16:11 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sigma/libft.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	while (n--)
		if (*(unsigned char *)s++ == (unsigned char)c)
			return ((void *)s - 1);
	return (NULL);
}

