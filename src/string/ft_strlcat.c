/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:27:44 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/24 19:23:52 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sigma/libft.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	srclen;
	size_t	dstlen;

	srclen = 0;
	dstlen = 0;
	while (dstlen < size && dst[dstlen])
		dstlen++;
	while (src[srclen])
		srclen++;
	if (size <= dstlen)
		return (srclen + size);
	i = 0;
	while (src[i] && dstlen + i < size - 1)
	{
		dst[dstlen + i] = src[i];
		i++;
	}
	dst[dstlen + i] = '\0';
	return (dstlen + srclen);
}

