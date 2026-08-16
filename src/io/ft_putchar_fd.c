/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:39:33 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/24 19:19:09 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.io.ft_putchar_fd
provides: libft.io.ft_putchar_fd
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void ft_putchar_fd(char c, int fd) { write(fd, &c, 1); }

/* sigma:end */
