/* sigma:begin
name: libft.memory.ft_memcmp
provides: libft.memory.ft_memcmp
deps:
externals:
kind: function
*/
#include <sigma/libft.h>

int ft_memcmp(const void *s1, const void *s2, size_t n) {
  unsigned char *temp1;
  unsigned char *temp2;

  temp1 = (unsigned char *)s1;
  temp2 = (unsigned char *)s2;
  if (!n)
    return (0);
  while (--n && *temp1 == *temp2) {
    temp1++;
    temp2++;
  }
  return ((int)*temp1 - *temp2);
}

/* sigma:end */
