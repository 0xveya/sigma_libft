#include <sigma/libft.h>

/* sigma:begin
name: libft.list.ft_lstdelone.ft_lstdelone
provides: libft.ft_lstdelone
deps:
externals:
kind: function
*/
void ft_lstdelone(t_list *lst, void (*del)(void *)) {
  if (lst) {
    del(lst->content);
    free(lst);
  }
}
/* sigma:end */
