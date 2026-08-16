#include <sigma/libft.h>
#include <sigma/reader.h>

#include <unistd.h>

/* sigma:begin
name: libft.reader.init
provides: io.reader.init
deps:
externals:
kind: function
*/
void sigma_line_reader_init(sigma_line_reader *reader, i32 fd,
                            allocator_t allocator) {
  *reader = (sigma_line_reader){.fd = fd, .allocator = allocator};
}
/* sigma:end */

/* sigma:begin
name: libft.reader.deinit
provides: io.reader.deinit
deps: mem.free
externals:
kind: function
*/
void sigma_line_reader_deinit(sigma_line_reader *reader) {
  if (reader->line_buffer != nullptr && reader->allocator.vtable != nullptr &&
      reader->allocator.vtable->free != nullptr)
    reader->allocator.vtable->free(reader->allocator.ctx, reader->line_buffer,
                                   reader->line_capacity,
                                   _Alignof(max_align_t));
  *reader = (sigma_line_reader){.fd = -1, .reached_eof = true};
}
/* sigma:end */

static sigma_line_result line_result(sigma_line_tag tag, char *items,
                                     usize len) {
  return (sigma_line_result){.tag = tag, .line = {.items = items, .len = len}};
}

static sigma_line_tag refill(sigma_line_reader *reader) {
  reader->read_position = 0;
  reader->read_length = 0;
  if (reader->reached_eof)
    return sigma_line_eof;
  isize amount =
      read(reader->fd, reader->read_buffer, sizeof(reader->read_buffer));
  if (amount < 0)
    return sigma_line_io_error;
  if (amount == 0) {
    reader->reached_eof = true;
    return sigma_line_eof;
  }
  reader->read_length = (usize)amount;
  return sigma_line_ok;
}

static bool reserve(sigma_line_reader *reader, usize required) {
  if (required <= reader->line_capacity)
    return true;
  usize capacity = reader->line_capacity == 0 ? SIGMA_READER_BUFFER_SIZE
                                              : reader->line_capacity;
  while (capacity < required) {
    if (capacity > SIZE_MAX / 2)
      return false;
    capacity *= 2;
  }
  if (reader->allocator.vtable == nullptr ||
      reader->allocator.vtable->alloc == nullptr)
    return false;
  char *buffer = reader->allocator.vtable->alloc(
      reader->allocator.ctx, capacity, _Alignof(max_align_t));
  if (buffer == nullptr)
    return false;
  ft_memcpy(buffer, reader->line_buffer, reader->line_capacity);
  if (reader->line_buffer != nullptr &&
      reader->allocator.vtable->free != nullptr)
    reader->allocator.vtable->free(reader->allocator.ctx, reader->line_buffer,
                                   reader->line_capacity,
                                   _Alignof(max_align_t));
  reader->line_buffer = buffer;
  reader->line_capacity = capacity;
  return true;
}

/* sigma:begin
name: libft.reader.next
provides: io.reader.next
deps: mem.alloc, mem.realloc, io.read
externals: read
kind: function
*/
sigma_line_result sigma_line_reader_next(sigma_line_reader *reader) {
  usize length = 0;

  for (;;) {
    if (reader->read_position == reader->read_length) {
      sigma_line_tag status = refill(reader);
      if (status != sigma_line_ok) {
        if (status == sigma_line_eof && length != 0)
          break;
        return line_result(status, nullptr, 0);
      }
    }
    usize available = reader->read_length - reader->read_position;
    usize chunk = 0;
    while (chunk < available &&
           reader->read_buffer[reader->read_position + chunk] != '\n')
      chunk++;
    if (length > SIZE_MAX - chunk - 1 || !reserve(reader, length + chunk + 1))
      return line_result(sigma_line_out_of_memory, nullptr, 0);
    ft_memcpy(reader->line_buffer + length,
              reader->read_buffer + reader->read_position, chunk);
    length += chunk;
    reader->read_position += chunk;
    if (reader->read_position < reader->read_length) {
      reader->read_position++;
      if (length != 0 && reader->line_buffer[length - 1] == '\r')
        length--;
      break;
    }
  }
  reader->line_buffer[length] = '\0';
  return line_result(sigma_line_ok, reader->line_buffer, length);
}
/* sigma:end */
