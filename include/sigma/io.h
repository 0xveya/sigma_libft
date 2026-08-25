#pragma once

#include <sigma/attributes.h>

/* Writes the formatted output to fd and returns its byte count or -1. */
int ft_fprintf(int fd, const char *format, ...) SIGMA_PRINTF(2, 3);
/* Writes the formatted output to stdout and returns its byte count or -1. */
int ft_printf(const char *format, ...) SIGMA_PRINTF(1, 2);
