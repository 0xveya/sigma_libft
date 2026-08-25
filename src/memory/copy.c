#include <sigma/mem.h>

/* sigma:begin
name: libft.memory.copy.mem_copy
provides: mem.copy
deps: libft.ft_memcpy
externals:
kind: function
*/
void *mem_copy(void *dst, const void *src, usize len) {
  return ft_memcpy(dst, src, len);
}
/* sigma:end */

/* sigma:begin
name: libft.memory.copy.sigma_memcpy
provides: sigma_malloc.memcpy
deps: mem.copy
externals:
kind: function
*/
void *sigma_memcpy(void *dst, const void *src, usize len) {
  return mem_copy(dst, src, len);
}
/* sigma:end */
