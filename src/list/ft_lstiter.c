#include <sigma/libft.h>

/* sigma:begin
name: libft.list.ft_lstiter.ft_lstiter
provides: libft.ft_lstiter
deps:
externals:
kind: function
*/
void ft_lstiter(t_list *lst, void (*f)(void *)) {
  while (lst) {
    f(lst->content);
    lst = lst->next;
  }
}
/* sigma:end */
