/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 18:58:29 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/24 19:15:49 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sigma:begin
name: libft.list.ft_lstsize
provides: libft.list.ft_lstsize
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

int ft_lstsize(t_list *lst) {
  int size;

  size = 0;
  while (lst) {
    size++;
    lst = lst->next;
  }
  return (size);
}

/* sigma:end */
