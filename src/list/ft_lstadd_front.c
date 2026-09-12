#include <sigma/libft.h>

/** Performs the ft lstadd front operation. */
void ft_lstadd_front(t_list **lst, t_list *new) {
  new->next = *lst;
  *lst = new;
}
