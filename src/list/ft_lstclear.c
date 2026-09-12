#include <sigma/libft.h>

/** Performs the ft lstclear operation. */
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
