/* sigma:begin
name: libft.list.ft_lstiter
provides: libft.list.ft_lstiter
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void ft_lstiter(t_list *lst, void (*f)(void *)) {
  while (lst) {
    f(lst->content);
    lst = lst->next;
  }
}

/* sigma:end */
