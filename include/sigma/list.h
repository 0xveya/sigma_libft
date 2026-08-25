#pragma once

#include <sigma/attributes.h>

typedef struct s_list {
  void *content;
  struct s_list *next;
} t_list;

/* Appends node to list, or makes it the first node when list is empty. */
void ft_lstadd_back(t_list **list, t_list *node);
/* Prepends node to list. */
void ft_lstadd_front(t_list **list, t_list *node);
/* Deletes every node and sets the list head to NULL. */
void ft_lstclear(t_list **list, void (*del)(void *));
/* Deletes one node after calling del for its content. */
void ft_lstdelone(t_list *node, void (*del)(void *));
/* Calls fn once for each node's content in list order. */
void ft_lstiter(t_list *list, void (*fn)(void *));
/* Returns the final node, or NULL when list is empty. */
SIGMA_NODISCARD t_list *ft_lstlast(t_list *list);
/* Allocates one node containing content, or returns NULL on failure. */
SIGMA_NODISCARD t_list *ft_lstnew(void *content);
/* Maps list into newly allocated nodes and cleans partial output on failure. */
SIGMA_NODISCARD t_list *ft_lstmap(t_list *list, void *(*fn)(void *),
                                  void (*del)(void *));
/* Returns the number of nodes in list. */
int ft_lstsize(t_list *list);
