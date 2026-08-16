#pragma once

#include <sigma/attributes.h>
#include <sigma/qol.h>

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
