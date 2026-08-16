/* sigma:begin
name: libft.io.ft_putnbr_fd
provides: libft.io.ft_putnbr_fd
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

void ft_putnbr_fd(int n, int fd) {
  char c;

  if (n == -2147483648) {
    write(fd, "-2147483648", 11);
    return;
  }
  if (n < 0) {
    write(fd, "-", 1);
    ft_putnbr_fd(-n, fd);
  } else if (n > 9) {
    ft_putnbr_fd(n / 10, fd);
    ft_putnbr_fd(n % 10, fd);
  } else {
    c = (char)(n + '0');
    write(fd, &c, 1);
  }
}
/* sigma:end */
