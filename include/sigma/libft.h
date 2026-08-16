#pragma once

#include <sigma/qol.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#if defined(__GNUC__) || defined(__clang__)
#define SIGMA_NODISCARD [[nodiscard]]
#define SIGMA_PRINTF(fmt, args) __attribute__((format(printf, fmt, args)))
#else
#define SIGMA_NODISCARD
#define SIGMA_PRINTF(fmt, args)
#endif

typedef struct s_list {
  void *content;
  struct s_list *next;
} t_list;

typedef enum sigma_parse_i32_tag {
  sigma_parse_i32_ok,
  sigma_parse_i32_invalid,
  sigma_parse_i32_overflow,
} sigma_parse_i32_tag;

typedef struct sigma_parse_i32_result {
  sigma_parse_i32_tag tag;
  i32 value;
} sigma_parse_i32_result;

SIGMA_NODISCARD sigma_parse_i32_result sigma_parse_i32(const char *text);
int ft_atoi(const char *text);
void ft_bzero(void *ptr, usize size);
SIGMA_NODISCARD void *ft_calloc(usize count, usize size);
int ft_isalnum(int c);
int ft_isalpha(int c);
int ft_isascii(int c);
int ft_isdigit(int c);
int ft_isprint(int c);
SIGMA_NODISCARD char *ft_itoa(int value);
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
SIGMA_NODISCARD void *ft_memchr(const void *ptr, int byte, usize size);
int ft_memcmp(const void *left, const void *right, usize size);
void *ft_memcpy(void *restrict dst, const void *restrict src, usize size);
void *ft_memmove(void *dst, const void *src, usize size);
void *ft_memset(void *ptr, int byte, usize size);
void ft_putchar_fd(char c, int fd);
void ft_putendl_fd(char *text, int fd);
void ft_putnbr_fd(int value, int fd);
void ft_putstr_fd(char *text, int fd);
SIGMA_NODISCARD char **ft_split(const char *text, char delimiter);
SIGMA_NODISCARD char *ft_strchr(const char *text, int c);
SIGMA_NODISCARD char *ft_strdup(const char *text);
void ft_striteri(char *text, void (*fn)(unsigned int, char *));
SIGMA_NODISCARD char *ft_strjoin(const char *left, const char *right);
usize ft_strlcat(char *dst, const char *src, usize size);
usize ft_strlcpy(char *dst, const char *src, usize size);
usize ft_strlen(const char *text);
SIGMA_NODISCARD char *ft_strmapi(const char *text,
                                 char (*fn)(unsigned int, char));
int ft_strncmp(const char *left, const char *right, usize size);
SIGMA_NODISCARD char *ft_strnstr(const char *text, const char *needle,
                                usize size);
SIGMA_NODISCARD char *ft_strrchr(const char *text, int c);
SIGMA_NODISCARD char *ft_strtrim(const char *text, const char *set);
SIGMA_NODISCARD char *ft_substr(const char *text, unsigned int start,
                               usize size);
int ft_tolower(int c);
int ft_toupper(int c);
int ft_printf(const char *format, ...) SIGMA_PRINTF(1, 2);
