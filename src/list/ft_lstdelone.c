/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 19:07:38 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/21 20:21:58 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.list.ft_lstdelone
provides: libft.list.ft_lstdelone
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void ft_lstdelone(t_list *lst, void (*del)(void *)) {
  if (lst) {
    del(lst->content);
    free(lst);
  }
}

/* sigma:end */
