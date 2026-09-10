#include <sigma/libft.h>
#include <sigma/sys.h>

[[gnu::always_inline]] static inline bool
sigma_writer_is_valid(sigma_writer writer) {
  return writer.vtable != NULL && writer.vtable->write != NULL;
}

static bool sigma_fd_writer_write(void *ctx, bytes_t bytes) {
  sigma_fd_writer *writer = ctx;
  usize offset = 0;

  while (offset < bytes.len) {
    sigma_write_result_t result =
        s_write(writer->fd, bytes.items + offset, bytes.len - offset);
    if (!result.ok || result.value == 0)
      return false;
    offset += result.value;
  }
  return true;
}

static bool sigma_string_writer_write(void *ctx, bytes_t bytes) {
  sigma_string_writer *writer = ctx;

  return string_append(writer->string, (str_t){
                                           .items = (const char *)bytes.items,
                                           .len = bytes.len,
                                       });
}

static bool sigma_fixed_writer_write(void *ctx, bytes_t bytes) {
  sigma_fixed_writer *writer = ctx;

  if (bytes.len > writer->cap - writer->len)
    return false;
  ft_memcpy(writer->items + writer->len, bytes.items, bytes.len);
  writer->len += bytes.len;
  return true;
}

static const sigma_writer_vtable sigma_fd_writer_vtable = {
    .write = sigma_fd_writer_write,
};

static const sigma_writer_vtable sigma_string_writer_vtable = {
    .write = sigma_string_writer_write,
};

static const sigma_writer_vtable sigma_fixed_writer_vtable = {
    .write = sigma_fixed_writer_write,
};

/* sigma:begin
name: libft.io.printf.support.writer.sigma_writer_write
provides: libft.sigma_writer_write
deps:
externals:
kind: function
*/
bool sigma_writer_write(sigma_writer writer, bytes_t bytes) {
  return sigma_writer_is_valid(writer) &&
         writer.vtable->write(writer.ctx, bytes);
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.support.writer.sigma_writer_str
provides: libft.sigma_writer_str
deps: libft.sigma_writer_write
externals:
kind: function
*/
bool sigma_writer_str(sigma_writer writer, str_t string) {
  return sigma_writer_write(writer, str_bytes(string));
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.support.writer.sigma_fd_writer_init
provides: libft.sigma_fd_writer_init
deps:
externals:
kind: function
*/
sigma_fd_writer sigma_fd_writer_init(int fd) {
  return (sigma_fd_writer){.fd = fd};
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.support.writer.sigma_fd_writer_as_writer
provides: libft.sigma_fd_writer_as_writer
deps:
externals:
kind: function
*/
sigma_writer sigma_fd_writer_as_writer(sigma_fd_writer *writer) {
  return (sigma_writer){.ctx = writer, .vtable = &sigma_fd_writer_vtable};
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.support.writer.sigma_string_writer_init
provides: libft.sigma_string_writer_init
deps:
externals:
kind: function
*/
sigma_string_writer sigma_string_writer_init(string_t *string) {
  return (sigma_string_writer){.string = string};
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.support.writer.sigma_string_writer_as_writer
provides: libft.sigma_string_writer_as_writer
deps:
externals:
kind: function
*/
sigma_writer sigma_string_writer_as_writer(sigma_string_writer *writer) {
  return (sigma_writer){.ctx = writer, .vtable = &sigma_string_writer_vtable};
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.support.writer.sigma_fixed_writer_init
provides: libft.sigma_fixed_writer_init
deps:
externals:
kind: function
*/
sigma_fixed_writer sigma_fixed_writer_init(bytes_mut_t buffer) {
  return (sigma_fixed_writer){
      .items = buffer.items,
      .len = 0,
      .cap = buffer.len,
  };
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.support.writer.sigma_fixed_writer_as_writer
provides: libft.sigma_fixed_writer_as_writer
deps:
externals:
kind: function
*/
sigma_writer sigma_fixed_writer_as_writer(sigma_fixed_writer *writer) {
  return (sigma_writer){.ctx = writer, .vtable = &sigma_fixed_writer_vtable};
}
/* sigma:end */

/* sigma:begin
name: libft.io.printf.support.writer.sigma_fixed_writer_written
provides: libft.sigma_fixed_writer_written
deps:
externals:
kind: function
*/
bytes_t sigma_fixed_writer_written(const sigma_fixed_writer *writer) {
  return (bytes_t){.items = writer->items, .len = writer->len};
}
/* sigma:end */
