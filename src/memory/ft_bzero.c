#include <sigma/libft.h>

/* sigma:begin
name: libft.memory.ft_bzero.ft_bzero
provides: mem.zero
deps: mem.set
externals:
kind: function
*/
void ft_bzero(void *s, size_t n) { (void)ft_memset(s, 0, n); }
/* sigma:end */
