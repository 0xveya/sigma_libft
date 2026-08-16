#pragma once

#include <sigma/attributes.h>

int ft_fprintf(int fd, const char *format, ...) SIGMA_PRINTF(2, 3);
int ft_printf(const char *format, ...) SIGMA_PRINTF(1, 2);
