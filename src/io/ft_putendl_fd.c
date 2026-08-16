/* sigma:begin
name: libft.io.ft_putendl_fd
provides: libft.io.ft_putendl_fd
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void ft_putendl_fd(char *s, int fd) {
  ft_putstr_fd(s, fd);
  write(fd, "\n", 1);
}

/* sigma:end */
