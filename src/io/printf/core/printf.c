#include "../printf_internal.h"

static void ft_print_next(t_writer *writer, const char *format, int *index,
                          va_list args) {
  t_format parsed;

  if (format[*index] != '%') {
    ft_writer_char(writer, format[*index]);
    (*index)++;
    return;
  }
  (*index)++;
  ft_format_init(&parsed);
  *index = ft_parse_format(format, *index, &parsed);
  ft_dispatch_print(writer, &parsed, args);
}

static int ft_vfprintf(int fd, const char *format, va_list args) {
  if (format == nullptr)
    return -1;
  t_writer writer = {.fd = fd};
  int index = 0;
  while (format[index] != '\0' && !writer.error)
    ft_print_next(&writer, format, &index, args);
  if (!writer.error)
    ft_writer_flush(&writer);
  return writer.error ? -1 : writer.total;
}

/* sigma:begin
name: libft.io.printf.core.printf.ft_fprintf
provides: io.fprintf
deps: io.write
externals:
kind: function
*/
int ft_fprintf(int fd, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int result = ft_vfprintf(fd, format, args);
  va_end(args);
  return result;
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.core.printf.ft_printf
provides: io.printf
deps: io.fprintf
externals:
kind: function
*/
int ft_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  int result = ft_vfprintf(STDOUT_FILENO, format, args);
  va_end(args);
  return result;
}
/* sigma:end */
