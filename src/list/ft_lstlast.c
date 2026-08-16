/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 18:59:53 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/21 20:22:04 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.list.ft_lstlast
provides: libft.list.ft_lstlast
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

t_list *ft_lstlast(t_list *lst) {
  while (lst && lst->next)
    lst = lst->next;
  return (lst);
}

/* sigma:end */
