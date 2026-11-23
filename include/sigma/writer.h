#pragma once

#include <sigma/bytes.h>
#include <sigma/str.h>
#ifndef SIGMA_RUNTIME_ONLY
#include <sigma/string.h>
#endif

#include <stdbool.h>

typedef struct sigma_writer sigma_writer;

typedef enum {
  sigma_writer_invalid,
  sigma_writer_io_error,
  sigma_writer_no_progress,
  sigma_writer_out_of_memory,
  sigma_writer_capacity_exceeded,
} sigma_writer_error;

RESULT(sigma_writer_status, sigma_unit_t, sigma_writer_error);

typedef struct {
  sigma_writer_status (*write)(void *ctx, bytes_t bytes);
} sigma_writer_vtable;

struct sigma_writer {
  void *ctx;
  const sigma_writer_vtable *vtable;
};

typedef struct {
  int fd;
} sigma_fd_writer;

#ifndef SIGMA_RUNTIME_ONLY
typedef struct {
  string_t *string;
} sigma_string_writer;
#endif

typedef struct {
  u8 *items;
  usize len;
  usize cap;
} sigma_fixed_writer;

SIGMA_NODISCARD sigma_writer_status sigma_writer_write(sigma_writer writer,
                                                       bytes_t bytes);

SIGMA_NODISCARD sigma_writer_status sigma_writer_str(sigma_writer writer,
                                                     str_t string);

sigma_fd_writer sigma_fd_writer_init(int fd);

sigma_writer sigma_fd_writer_as_writer(sigma_fd_writer *writer);

#ifndef SIGMA_RUNTIME_ONLY
sigma_string_writer sigma_string_writer_init(string_t *string);

sigma_writer sigma_string_writer_as_writer(sigma_string_writer *writer);
#endif

sigma_fixed_writer sigma_fixed_writer_init(bytes_mut_t buffer);

sigma_writer sigma_fixed_writer_as_writer(sigma_fixed_writer *writer);

bytes_t sigma_fixed_writer_written(const sigma_fixed_writer *writer);
