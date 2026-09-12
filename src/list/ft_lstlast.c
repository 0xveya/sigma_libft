#include <sigma/libft.h>

/** Performs the ft lstlast operation. */
t_list *ft_lstlast(t_list *lst) {
  while (lst && lst->next)
    lst = lst->next;
  return (lst);
}
