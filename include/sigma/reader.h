#pragma once

#include <allocator_vtable.h>
#include <sigma/attributes.h>
#include <sigma/slice.h>
#include <stdbool.h>

#define SIGMA_READER_BUFFER_SIZE 4096u

typedef enum sigma_line_tag {
  sigma_line_ok,
  sigma_line_eof,
  sigma_line_io_error,
  sigma_line_out_of_memory,
} sigma_line_tag;

typedef struct sigma_line_result {
  sigma_line_tag tag;
  str_t line;
} sigma_line_result;

typedef struct sigma_line_reader {
  i32 fd;
  u8 read_buffer[SIGMA_READER_BUFFER_SIZE];
  usize read_position;
  usize read_length;
  char *line_buffer;
  usize line_capacity;
  bool reached_eof;
  allocator_t allocator;
} sigma_line_reader;

void sigma_line_reader_init(sigma_line_reader *reader, i32 fd,
                            allocator_t allocator);
void sigma_line_reader_deinit(sigma_line_reader *reader);
SIGMA_NODISCARD sigma_line_result
sigma_line_reader_next(sigma_line_reader *reader);
