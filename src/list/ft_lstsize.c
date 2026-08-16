#include <sigma/libft.h>

/* sigma:begin
name: libft.list.ft_lstsize.ft_lstsize
provides: libft.ft_lstsize
deps:
externals:
kind: function
*/
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
