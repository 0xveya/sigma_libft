/* sigma:begin
name: libft.io.ft_putstr_fd
provides: libft.io.ft_putstr_fd
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void ft_putstr_fd(char *s, int fd) { write(fd, s, ft_strlen(s)); }

/* sigma:end */
