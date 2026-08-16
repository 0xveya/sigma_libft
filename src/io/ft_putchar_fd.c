/* sigma:begin
name: libft.io.ft_putchar_fd
provides: libft.io.ft_putchar_fd
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void ft_putchar_fd(char c, int fd) { write(fd, &c, 1); }

/* sigma:end */
