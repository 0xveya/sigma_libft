#include <sigma/libft.h>

/** Performs the ft lstsize operation. */
int ft_lstsize(t_list *lst) {
  int size;

  size = 0;
  while (lst) {
    size++;
    lst = lst->next;
  }
  return (size);
}
