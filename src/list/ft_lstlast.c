/* sigma:begin
name: libft.list.ft_lstlast
provides: libft.list.ft_lstlast
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

t_list *ft_lstlast(t_list *lst) {
  while (lst && lst->next)
    lst = lst->next;
  return (lst);
}

/* sigma:end */
