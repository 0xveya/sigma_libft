#pragma once

#include <sigma/attributes.h>

typedef struct s_list {
  void *content;
  struct s_list *next;
} t_list;

void ft_lstadd_back(t_list **list, t_list *node);
void ft_lstadd_front(t_list **list, t_list *node);
void ft_lstclear(t_list **list, void (*del)(void *));
void ft_lstdelone(t_list *node, void (*del)(void *));
void ft_lstiter(t_list *list, void (*fn)(void *));
SIGMA_NODISCARD t_list *ft_lstlast(t_list *list);
SIGMA_NODISCARD t_list *ft_lstnew(void *content);
SIGMA_NODISCARD t_list *ft_lstmap(t_list *list, void *(*fn)(void *),
                                  void (*del)(void *));
int ft_lstsize(t_list *list);
