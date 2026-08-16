/* sigma:begin
name: libft.list.ft_lstclear
provides: libft.list.ft_lstclear
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void ft_lstclear(t_list **lst, void (*del)(void *)) {
  t_list *tmp;

  if (!lst || !(*lst))
    return;
  while (*lst) {
    tmp = (*lst)->next;
    del((*lst)->content);
    free(*lst);
    *lst = tmp;
  }
}

/* sigma:end */
