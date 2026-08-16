/* sigma:begin
name: libft.list.ft_lstdelone
provides: libft.list.ft_lstdelone
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void ft_lstdelone(t_list *lst, void (*del)(void *)) {
  if (lst) {
    del(lst->content);
    free(lst);
  }
}

/* sigma:end */
