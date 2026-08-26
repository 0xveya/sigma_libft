#pragma once

#include <sigma/bytes.h>
#include <sigma/str.h>
#include <sigma/string.h>

#include <stdbool.h>

typedef struct sigma_writer sigma_writer;

typedef struct {
  bool (*write)(void *ctx, bytes_t bytes);
} sigma_writer_vtable;

struct sigma_writer {
  void *ctx;
  const sigma_writer_vtable *vtable;
};

typedef struct {
  int fd;
} sigma_fd_writer;

typedef struct {
  string_t *string;
} sigma_string_writer;

typedef struct {
  u8 *items;
  usize len;
  usize cap;
} sigma_fixed_writer;

bool sigma_writer_write(sigma_writer writer, bytes_t bytes);

bool sigma_writer_str(sigma_writer writer, str_t string);

sigma_fd_writer sigma_fd_writer_init(int fd);

sigma_writer sigma_fd_writer_as_writer(sigma_fd_writer *writer);

sigma_string_writer sigma_string_writer_init(string_t *string);

sigma_writer sigma_string_writer_as_writer(sigma_string_writer *writer);

sigma_fixed_writer sigma_fixed_writer_init(bytes_mut_t buffer);

sigma_writer sigma_fixed_writer_as_writer(sigma_fixed_writer *writer);

bytes_t sigma_fixed_writer_written(const sigma_fixed_writer *writer);
