#include <sigma/libft.h>

static int ft_numlen(long n) {
  int len;

  len = 0;
  if (n <= 0)
    len = 1;
  while (n) {
    len++;
    n /= 10;
  }
  return (len);
}

/* sigma:begin
name: libft.conversion.ft_itoa.ft_itoa
provides: libft.ft_itoa
deps:
externals:
kind: function
*/
char *ft_itoa(int n) {
  char *str;
  long nb;
  int len;

  nb = n;
  len = ft_numlen(nb);
  str = malloc((usize)len + 1);
  if (!str)
    return (NULL);
  str[len] = '\0';
  if (nb == 0)
    str[0] = '0';
  if (nb < 0) {
    str[0] = '-';
    nb = -nb;
  }
  while (nb > 0) {
    str[--len] = (nb % 10) + '0';
    nb /= 10;
  }
  return (str);
}
/* sigma:end */
