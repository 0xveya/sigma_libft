#include <sigma/libft.h>

/* sigma:begin
name: libft.strlen
provides: str.len
deps:
externals:
kind: function
*/
usize ft_strlen(const char *text) {
  const char *end = text;

  while (*end != '\0')
    end++;
  return (usize)(end - text);
}
/* sigma:end */
