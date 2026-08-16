#include <sigma/libft.h>

/* sigma:begin
name: libft.list.ft_lstclear.ft_lstclear
provides: libft.ft_lstclear
deps:
externals:
kind: function
*/
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
