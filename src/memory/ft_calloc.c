#include <sigma/libft.h>
#include <stdint.h>

/* sigma:begin
name: libft.memory.ft_calloc.ft_calloc
provides: libft.ft_calloc
deps:
externals:
kind: function
*/
void *ft_calloc(size_t n, size_t size) {
  void *ptr;
  size_t total;

  if (n != 0 && size > SIZE_MAX / n)
    return (NULL);
  total = n * size;
  if (total == 0)
    total = 1;
  ptr = malloc(total);
  if (!ptr)
    return (NULL);
  return (ft_bzero(ptr, total), ptr);
}
/* sigma:end */
