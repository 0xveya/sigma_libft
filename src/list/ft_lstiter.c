#include <sigma/libft.h>

/** Performs the ft lstiter operation. */
void ft_lstiter(t_list *lst, void (*f)(void *)) {
  while (lst) {
    f(lst->content);
    lst = lst->next;
  }
}
