#include <sigma/libft.h>

/* sigma:begin
name: libft.list.ft_lstlast.ft_lstlast
provides: libft.ft_lstlast
deps:
externals:
kind: function
*/
t_list *ft_lstlast(t_list *lst) {
  while (lst && lst->next)
    lst = lst->next;
  return (lst);
}
/* sigma:end */
