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
