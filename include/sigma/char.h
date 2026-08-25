#pragma once

/* Returns nonzero when c is an ASCII letter or decimal digit. */
int ft_isalnum(int c);
/* Returns nonzero when c is an ASCII letter. */
int ft_isalpha(int c);
/* Returns nonzero when c is representable as seven-bit ASCII. */
int ft_isascii(int c);
/* Returns nonzero when c is an ASCII decimal digit. */
int ft_isdigit(int c);
/* Returns nonzero when c is a printable ASCII character. */
int ft_isprint(int c);
/* Converts an uppercase ASCII letter to lowercase. */
int ft_tolower(int c);
/* Converts a lowercase ASCII letter to uppercase. */
int ft_toupper(int c);
