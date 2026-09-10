#include "format_internal.h"
#include <unistd.h>

/* sigma:begin
name: libft.io.printf.format.format.sigma_fmt_write_repeat
provides: libft.sigma_fmt_write_repeat
deps: libft.sigma_writer_str
externals:
kind: function
*/
bool sigma_fmt_write_repeat(sigma_writer writer, u32 fill, usize count) {
  char buffer[64];

  if (fill > UINT8_MAX)
    panic("non-ASCII format fill is not implemented");
  for (usize index = 0; index < sizeof(buffer); index++)
    buffer[index] = (char)fill;
  while (count > 0) {
    usize chunk = count < sizeof(buffer) ? count : sizeof(buffer);

    if (!sigma_writer_str(writer, (str_t){.items = buffer, .len = chunk}))
      return false;
    count -= chunk;
  }
  return true;
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.format.format.sigma_fmt_write_arg
provides: libft.sigma_fmt_write_arg
deps: libft.sigma_fmt_write_i64 libft.sigma_fmt_write_u64
libft.sigma_fmt_write_str externals: kind: function
*/
bool sigma_fmt_write_arg(sigma_writer writer, sigma_fmt_arg_t arg,
                         const sigma_fmt_spec *spec) {
  switch (arg.kind) {
  case SIGMA_FMT_BOOL:
    return sigma_fmt_write_bool(writer, arg.value.boolean, spec);

  case SIGMA_FMT_I64:
    return sigma_fmt_write_i64(writer, arg.value.signed_integer, spec);

  case SIGMA_FMT_U64:
    return sigma_fmt_write_u64(writer, arg.value.unsigned_integer, spec);

  case SIGMA_FMT_STR:
    return sigma_fmt_write_str(writer, arg.value.str, spec);

  case SIGMA_FMT_CHAR:
    return sigma_fmt_write_char(writer, arg.value.character, spec);

  case SIGMA_FMT_RUNE:
    return sigma_fmt_write_rune(writer, arg.value.rune, spec);

  case SIGMA_FMT_BYTES:
    return sigma_fmt_write_bytes(writer, arg.value.bytes, spec);

  case SIGMA_FMT_PTR:
    return sigma_fmt_write_ptr(writer, arg.value.pointer, spec);

  case SIGMA_FMT_CUSTOM:
    if (arg.value.custom.value == NULL || arg.value.custom.vtable == NULL ||
        arg.value.custom.vtable->format == NULL)
      return false;
    return arg.value.custom.vtable->format(writer, arg.value.custom.value,
                                           spec);

  case SIGMA_FMT_F64:
  default:
    return false;
  }
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.format.format.sigma_format_args
provides: libft.sigma_format_args
deps: libft.sigma_fmt_parse_field libft.sigma_fmt_write_arg
libft.sigma_writer_str externals: kind: function
*/
bool sigma_format_args(sigma_writer writer, str_t format,
                       const sigma_fmt_arg_t *args, usize arg_count) {
  usize pos = 0;
  usize arg = 0;
  usize literal_start = 0;

  while (pos < format.len) {
    if (format.items[pos] != '{') {
      pos++;
      continue;
    }

    if (pos > literal_start) {
      str_t literal = {
          .items = format.items + literal_start,
          .len = pos - literal_start,
      };

      if (!sigma_writer_str(writer, literal))
        return false;
    }

    if (arg >= arg_count)
      return false;

    sigma_fmt_field field;

    sigma_fmt_parse_result result = sigma_fmt_parse_field(
        (str_t){
            .items = format.items + pos,
            .len = format.len - pos,
        },
        &field);

    if (result != SIGMA_FMT_PARSE_OK)
      return false;

    if (!sigma_fmt_write_arg(writer, args[arg], &field.spec))
      return false;

    arg++;
    pos += field.consumed;
    literal_start = pos;
  }

  if (literal_start < format.len) {
    str_t literal = {
        .items = format.items + literal_start,
        .len = format.len - literal_start,
    };

    if (!sigma_writer_str(writer, literal))
      return false;
  }

  return arg == arg_count;
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.format.format.sigma_fprint_args
provides: libft.sigma_fprint_args
deps: libft.sigma_fd_writer_init libft.sigma_fd_writer_as_writer
libft.sigma_format_args
externals:
kind: function
*/
bool sigma_fprint_args(int fd, str_t format, const sigma_fmt_arg_t *args,
                       usize arg_count) {
  sigma_fd_writer writer = sigma_fd_writer_init(fd);

  return sigma_format_args(sigma_fd_writer_as_writer(&writer), format, args,
                           arg_count);
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.format.format.sigma_snprint_args
provides: libft.sigma_snprint_args
deps: libft.sigma_fixed_writer_init libft.sigma_fixed_writer_as_writer
libft.sigma_fixed_writer_written libft.sigma_format_args
externals:
kind: function
*/
bool sigma_snprint_args(char *buffer, usize capacity, str_t format,
                        const sigma_fmt_arg_t *args, usize arg_count) {
  if (buffer == NULL || capacity == 0)
    return false;
  buffer[0] = '\0';
  sigma_fixed_writer writer = sigma_fixed_writer_init((bytes_mut_t){
      .items = (u8 *)buffer,
      .len = capacity - 1,
  });

  if (!sigma_format_args(sigma_fixed_writer_as_writer(&writer), format, args,
                         arg_count))
    return false;
  bytes_t written = sigma_fixed_writer_written(&writer);
  buffer[written.len] = '\0';
  return true;
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.format.format.sigma_asprint_args
provides: libft.sigma_asprint_args
deps: libft.string_init libft.string_deinit libft.sigma_string_writer_init
libft.sigma_string_writer_as_writer libft.sigma_format_args
externals:
kind: function
*/
bool sigma_asprint_args(string_t *out, allocator_t allocator, str_t format,
                        const sigma_fmt_arg_t *args, usize arg_count) {
  if (out == NULL)
    return false;
  string_t result = string_init(allocator);
  sigma_string_writer writer = sigma_string_writer_init(&result);

  if (!sigma_format_args(sigma_string_writer_as_writer(&writer), format, args,
                         arg_count)) {
    string_deinit(&result);
    return false;
  }
  *out = result;
  return true;
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.format.format.sigma_print_args
provides: libft.sigma_print_args
deps: libft.sigma_fprint_args
externals: STDOUT_FILENO
kind: function
*/
bool sigma_print_args(str_t format, const sigma_fmt_arg_t *args,
                      usize arg_count) {
  return sigma_fprint_args(STDOUT_FILENO, format, args, arg_count);
}
/* sigma:end */
