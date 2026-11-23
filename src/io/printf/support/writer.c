#include <sigma/mem.h>
#include <sigma/writer.h>
#include <sigma/sys.h>

[[gnu::always_inline]] static inline bool
sigma_writer_is_valid(sigma_writer writer) {
  return writer.vtable != NULL && writer.vtable->write != NULL;
}

static sigma_writer_status sigma_fd_writer_write(void *ctx, bytes_t bytes) {
  sigma_fd_writer *writer = ctx;
  usize offset = 0;

  while (offset < bytes.len) {
    sigma_write_result_t result =
        s_write(writer->fd, bytes.items + offset, bytes.len - offset);
    if (!result.ok)
      return ERR(sigma_writer_status, sigma_writer_io_error);
    if (result.value == 0)
      return ERR(sigma_writer_status, sigma_writer_no_progress);
    offset += result.value;
  }
  return OK(sigma_writer_status, sigma_unit);
}

#ifndef SIGMA_RUNTIME_ONLY
static sigma_writer_status sigma_string_writer_write(void *ctx, bytes_t bytes) {
  sigma_string_writer *writer = ctx;

  sigma_alloc_status status =
      string_append(writer->string, (str_t){.items = (const char *)bytes.items,
                                            .len = bytes.len});
  if (!status.ok) {
    sigma_writer_error error = status.error == sigma_alloc_size_overflow
                                   ? sigma_writer_capacity_exceeded
                                   : sigma_writer_out_of_memory;
    return ERR(sigma_writer_status, error);
  }
  return OK(sigma_writer_status, sigma_unit);
}
#endif

static sigma_writer_status sigma_fixed_writer_write(void *ctx, bytes_t bytes) {
  sigma_fixed_writer *writer = ctx;

  if (bytes.len > writer->cap - writer->len)
    return ERR(sigma_writer_status, sigma_writer_capacity_exceeded);
  ft_memcpy(writer->items + writer->len, bytes.items, bytes.len);
  writer->len += bytes.len;
  return OK(sigma_writer_status, sigma_unit);
}

static const sigma_writer_vtable sigma_fd_writer_vtable = {
    .write = sigma_fd_writer_write,
};

#ifndef SIGMA_RUNTIME_ONLY
static const sigma_writer_vtable sigma_string_writer_vtable = {
    .write = sigma_string_writer_write,
};
#endif

static const sigma_writer_vtable sigma_fixed_writer_vtable = {
    .write = sigma_fixed_writer_write,
};

/** Performs the sigma writer write operation. */
sigma_writer_status sigma_writer_write(sigma_writer writer, bytes_t bytes) {
  if (!sigma_writer_is_valid(writer))
    return ERR(sigma_writer_status, sigma_writer_invalid);
  return writer.vtable->write(writer.ctx, bytes);
}

/** Performs the sigma writer str operation. */
sigma_writer_status sigma_writer_str(sigma_writer writer, str_t string) {
  return sigma_writer_write(writer, str_bytes(string));
}

/** Performs the sigma fd writer init operation. */
sigma_fd_writer sigma_fd_writer_init(int fd) {
  return (sigma_fd_writer){.fd = fd};
}

/** Performs the sigma fd writer as writer operation. */
sigma_writer sigma_fd_writer_as_writer(sigma_fd_writer *writer) {
  return (sigma_writer){.ctx = writer, .vtable = &sigma_fd_writer_vtable};
}

#ifndef SIGMA_RUNTIME_ONLY
/** Performs the sigma string writer init operation. */
sigma_string_writer sigma_string_writer_init(string_t *string) {
  return (sigma_string_writer){.string = string};
}

/** Performs the sigma string writer as writer operation. */
sigma_writer sigma_string_writer_as_writer(sigma_string_writer *writer) {
  return (sigma_writer){.ctx = writer, .vtable = &sigma_string_writer_vtable};
}
#endif

/** Performs the sigma fixed writer init operation. */
sigma_fixed_writer sigma_fixed_writer_init(bytes_mut_t buffer) {
  return (sigma_fixed_writer){
      .items = buffer.items,
      .len = 0,
      .cap = buffer.len,
  };
}

/** Performs the sigma fixed writer as writer operation. */
sigma_writer sigma_fixed_writer_as_writer(sigma_fixed_writer *writer) {
  return (sigma_writer){.ctx = writer, .vtable = &sigma_fixed_writer_vtable};
}

/** Performs the sigma fixed writer written operation. */
bytes_t sigma_fixed_writer_written(const sigma_fixed_writer *writer) {
  return (bytes_t){.items = writer->items, .len = writer->len};
}
