/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 18:52:11 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/21 20:22:10 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.list.ft_lstnew
provides: libft.list.ft_lstnew
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

t_list *ft_lstnew(void *content) {
  t_list *new;

  new = (t_list *)malloc(sizeof(t_list));
  if (!new)
    return (NULL);
  new->content = content;
  new->next = NULL;
  return (new);
}

/* sigma:end */
