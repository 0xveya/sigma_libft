#pragma once

#include <sigma/attributes.h>

void ft_putchar_fd(char c, int fd);
void ft_putendl_fd(char *text, int fd);
void ft_putnbr_fd(int value, int fd);
void ft_putstr_fd(char *text, int fd);
int ft_printf(const char *format, ...) SIGMA_PRINTF(1, 2);
