#include <sigma/libft.h>
#include <sigma/reader.h>

#include "reader_internal.h"

#include <unistd.h>

typedef struct reader_arena_block {
  struct reader_arena_block *next;
  usize allocation_size;
  usize used;
  usize capacity;
  u8 data[];
} reader_arena_block;

static void *reader_arena_alloc(void *context, usize size, usize alignment) {
  allocator_arena_t *arena = context;
  if (size == 0 || alignment == 0 || (alignment & (alignment - 1)) != 0 ||
      size > SIZE_MAX - alignment)
    return nullptr;
  reader_arena_block *block = arena->blocks;
  if (block != nullptr) {
    uptr current = (uptr)block->data + block->used;
    uptr aligned = (current + alignment - 1) & ~(uptr)(alignment - 1);
    usize padding = (usize)(aligned - current);
    if (padding <= block->capacity - block->used &&
        size <= block->capacity - block->used - padding) {
      block->used += padding + size;
      return (void *)aligned;
    }
  }
  usize capacity = arena->block_size;
  usize required = size + alignment - 1;
  if (capacity < required)
    capacity = required;
  if (capacity > SIZE_MAX - sizeof(reader_arena_block) ||
      arena->parent.vtable == nullptr || arena->parent.vtable->alloc == nullptr)
    return nullptr;
  usize allocation_size = sizeof(reader_arena_block) + capacity;
  block = arena->parent.vtable->alloc(arena->parent.ctx, allocation_size,
                                      _Alignof(max_align_t));
  if (block == nullptr)
    return nullptr;
  *block = (reader_arena_block){
      .next = arena->blocks,
      .allocation_size = allocation_size,
      .capacity = capacity,
  };
  arena->blocks = block;
  uptr aligned = ((uptr)block->data + alignment - 1) & ~(uptr)(alignment - 1);
  block->used = (usize)(aligned - (uptr)block->data) + size;
  return (void *)aligned;
}

static void reader_arena_free(void *context, void *ptr, usize size,
                              usize alignment) {
  (void)context;
  (void)ptr;
  (void)size;
  (void)alignment;
}

static const allocator_vtable_t reader_arena_vtable = {
    .alloc = reader_arena_alloc,
    .free = reader_arena_free,
};

static void reader_arena_deinit(allocator_arena_t *arena) {
  reader_arena_block *block = arena->blocks;
  while (block != nullptr) {
    reader_arena_block *next = block->next;
    if (arena->parent.vtable != nullptr &&
        arena->parent.vtable->free != nullptr)
      arena->parent.vtable->free(arena->parent.ctx, block,
                                 block->allocation_size, _Alignof(max_align_t));
    block = next;
  }
  arena->blocks = nullptr;
}

/* sigma:begin
name: libft.reader.reader.sigma_line_reader_init
provides: io.reader.init
deps: mem.alloc
externals:
kind: function
*/
void sigma_line_reader_init(sigma_line_reader *reader, i32 fd,
                            allocator_t allocator) {
  *reader = (sigma_line_reader){
      .fd = fd,
      .arena = {.parent = allocator, .block_size = SIGMA_READER_BUFFER_SIZE},
  };
  reader->allocator = (allocator_t){
      .ctx = &reader->arena,
      .vtable = &reader_arena_vtable,
  };
}
/* sigma:end */

/* sigma:begin
name: libft.reader.reader.sigma_line_reader_deinit
provides: io.reader.deinit
deps: mem.free
externals:
kind: function
*/
void sigma_line_reader_deinit(sigma_line_reader *reader) {
  reader_arena_deinit(&reader->arena);
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
name: libft.reader.reader.sigma_line_reader_next
provides: io.reader.next
deps: mem.alloc, mem.copy, io.read, io.reader.scan
externals:
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
    usize chunk = sigma_reader_find_newline(
        reader->read_buffer + reader->read_position, available);
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
