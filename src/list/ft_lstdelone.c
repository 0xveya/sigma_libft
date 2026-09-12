#include <sigma/libft.h>

/** Performs the ft lstdelone operation. */
void ft_lstdelone(t_list *lst, void (*del)(void *)) {
  if (lst) {
    del(lst->content);
    free(lst);
  }
}
