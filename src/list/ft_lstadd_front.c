#include <sigma/libft.h>

/* sigma:begin
name: libft.list.ft_lstadd_front.ft_lstadd_front
provides: libft.ft_lstadd_front
deps:
externals:
kind: function
*/
void ft_lstadd_front(t_list **lst, t_list *new) {
  new->next = *lst;
  *lst = new;
}
/* sigma:end */
