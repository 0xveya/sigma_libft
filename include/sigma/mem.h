#pragma once

#include <sigma/attributes.h>
#include <sigma/qol.h>

void ft_bzero(void *ptr, usize size);
SIGMA_NODISCARD void *ft_calloc(usize count, usize size);
SIGMA_NODISCARD void *ft_memchr(const void *ptr, int byte, usize size);
int ft_memcmp(const void *left, const void *right, usize size);
void *ft_memcpy(void *restrict dst, const void *restrict src, usize size);
void *ft_memmove(void *dst, const void *src, usize size);
void *ft_memset(void *ptr, int byte, usize size);
