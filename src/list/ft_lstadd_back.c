#include <sigma/libft.h>

/* sigma:begin
name: libft.list.ft_lstadd_back.ft_lstadd_back
provides: libft.ft_lstadd_back
deps:
externals:
kind: function
*/
void ft_lstadd_back(t_list **lst, t_list *new) {
  t_list *tmp;

  if (*lst == NULL)
    *lst = new;
  else {
    tmp = *lst;
    while (tmp->next != NULL)
      tmp = tmp->next;
    tmp->next = new;
  }
}
/* sigma:end */
