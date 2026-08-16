/* sigma:begin
name: libft.list.ft_lstnew
provides: libft.list.ft_lstnew
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

t_list *ft_lstnew(void *content) {
  t_list *new;

  new = (t_list *)malloc(sizeof(t_list));
  if (!new)
    return (NULL);
  new->content = content;
  new->next = NULL;
  return (new);
}

/* sigma:end */
