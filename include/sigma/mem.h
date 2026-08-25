#pragma once

#include <sigma/attributes.h>
#include <sigma/qol.h>

/* Returns the first matching byte offset, or SIGMA_NPOS when absent. */
usize mem_find(const void *ptr, usize len, u8 value);

/* Compares exactly len bytes as unsigned values. */
i32 mem_cmp(const void *a, const void *b, usize len);

/* Copies exactly len non-overlapping bytes and returns dst. */
void *mem_copy(void *dst, const void *src, usize len);

/* Moves exactly len potentially overlapping bytes and returns dst. */
void *mem_move(void *dst, const void *src, usize len);

/* Sets exactly len bytes and returns dst. */
void *mem_set(void *dst, u8 value, usize len);

void ft_bzero(void *ptr, usize size);
SIGMA_NODISCARD void *ft_calloc(usize count, usize size);
SIGMA_NODISCARD void *ft_memchr(const void *ptr, int byte, usize size);
int ft_memcmp(const void *left, const void *right, usize size);
void *ft_memcpy(void *restrict dst, const void *restrict src, usize size);
void *ft_memmove(void *dst, const void *src, usize size);
void *ft_memset(void *ptr, int byte, usize size);
