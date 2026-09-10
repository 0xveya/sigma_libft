#include <sigma/libft.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHECK(condition)                                                       \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr, "check failed at %s:%d: %s\n", __FILE__, __LINE__,       \
              #condition);                                                     \
      return false;                                                            \
    }                                                                          \
  } while (0)

static usize alloc_count;
static usize free_count;

static void *test_alloc(void *ctx, usize size, usize alignment) {
  (void)ctx;
  (void)alignment;
  ++alloc_count;
  return malloc(size);
}

static void test_free(void *ctx, void *ptr, usize size, usize alignment) {
  (void)ctx;
  (void)size;
  (void)alignment;
  ++free_count;
  free(ptr);
}

static allocator_t test_allocator(void) {
  static const allocator_vtable_t vtable = {.alloc = test_alloc,
                                            .free = test_free};
  return (allocator_t){.vtable = &vtable};
}

static void noop_delete(void *ptr) { (void)ptr; }
static usize iter_count;
static void count_node(void *ptr) {
  (void)ptr;
  ++iter_count;
}
static void *identity(void *ptr) { return ptr; }

bool sigma_test_conversion_and_memory(void) {
  sigma_parse_i32_result min = sigma_parse_i32("-2147483648");
  CHECK(min.tag == sigma_parse_i32_ok && min.value == INT32_MIN);
  CHECK(sigma_parse_i32("12nope").tag == sigma_parse_i32_invalid);
  CHECK(sigma_parse_i32("2147483648").tag == sigma_parse_i32_overflow);
  CHECK(ft_atoi("-42") == -42);

  char *integer = ft_itoa(INT32_MIN);
  CHECK(integer != NULL && strcmp(integer, "-2147483648") == 0);
  free(integer);

  u8 source[193];
  u8 destination[197];
  for (usize index = 0; index < sizeof(source); ++index)
    source[index] = (u8)(index * 17U);
  memset(destination, 0xcc, sizeof(destination));
  CHECK(ft_memcpy(destination + 2, source, sizeof(source)) == destination + 2);
  CHECK(destination[1] == 0xcc && destination[195] == 0xcc);
  CHECK(memcmp(destination + 2, source, sizeof(source)) == 0);
  CHECK(ft_memset(destination + 3, 0x5a, 191) == destination + 3);
  CHECK(destination[2] == source[0] && destination[194] == source[192]);

  u8 overlap[] = {'a', 'b', 'c', 'd', 0};
  ft_memmove(overlap + 1, overlap, 3);
  CHECK(memcmp(overlap, "aabc", 4) == 0);
  CHECK(ft_memchr("abc", 'b', 3) != NULL);
  CHECK(ft_memcmp("abc", "abd", 3) < 0);
  ft_bzero(destination, 17);
  for (usize index = 0; index < 17; ++index)
    CHECK(destination[index] == 0);
  void *zeroed = ft_calloc(17, 1);
  CHECK(zeroed != NULL && mem_cmp(zeroed, (u8[17]){0}, 17) == 0);
  free(zeroed);

  const u8 left[] = {1, 2, 3, 4};
  const u8 right[] = {1, 2, 4, 0};
  CHECK(mem_find(left, 3, 3) == 2 && mem_find(left, 2, 3) == SIGMA_NPOS);
  CHECK(mem_find(NULL, 0, 0) == SIGMA_NPOS);
  CHECK(mem_cmp(left, right, 2) == 0 && mem_cmp(left, right, 3) < 0);

  u8 simd_left[257];
  u8 simd_right[257];
  memset(simd_left, 0x5a, sizeof(simd_left));
  memcpy(simd_right, simd_left, sizeof(simd_left));
  const usize boundaries[] = {0, 15, 16, 31, 32, 63, 64, 127, 128, 255, 256};
  for (usize index = 0; index < sizeof(boundaries) / sizeof(*boundaries);
       ++index) {
    usize position = boundaries[index];
    ++simd_right[position];
    CHECK(mem_cmp(simd_left, simd_right, sizeof(simd_left)) < 0);
    simd_right[position] = simd_left[position];
  }
  simd_left[256] = 0xff;
  CHECK(mem_find(simd_left, sizeof(simd_left), 0xff) == 256);
  CHECK(mem_find(simd_left, 256, 0xff) == SIGMA_NPOS);
  return true;
}

bool sigma_test_lists(void) {
  int value = 42;
  t_list *allocated = ft_lstnew(&value);
  CHECK(allocated != NULL && allocated->content == &value &&
        allocated->next == NULL);
  free(allocated);

  t_list last = {0};
  t_list first = {.next = &last};
  t_list front = {0};
  t_list back = {0};
  t_list *list = &first;
  ft_lstadd_front(&list, &front);
  CHECK(list == &front && front.next == &first);
  ft_lstadd_back(&list, &back);
  CHECK(last.next == &back && ft_lstlast(list) == &back &&
        ft_lstsize(list) == 4);
  iter_count = 0;
  ft_lstiter(list, count_node);
  CHECK(iter_count == 4);

  t_list *owned = ft_lstnew(NULL);
  CHECK(owned != NULL);
  owned->next = ft_lstnew(NULL);
  CHECK(owned->next != NULL);
  ft_lstclear(&owned, noop_delete);
  CHECK(owned == NULL);

  t_list map_last = {0};
  t_list map_first = {.next = &map_last};
  t_list *mapped = ft_lstmap(&map_first, identity, noop_delete);
  CHECK(mapped != NULL && ft_lstsize(mapped) == 2);
  ft_lstclear(&mapped, noop_delete);
  return true;
}

bool sigma_test_strings_and_bytes(void) {
  str_t text = str_from_cstr("alphabet");
  CHECK(text.len == 8 && !str_is_empty(text));
  CHECK(str_is_empty(str_sub(text, text.len, 10)));
  CHECK(str_eq(str_sub(text, 2, 3), STR_LIT("pha")));
  CHECK(str_eq(str_sub(text, 5, 99), STR_LIT("bet")));
  CHECK(str_cmp(STR_LIT("abc"), STR_LIT("abd")) < 0);
  CHECK(str_starts_with(text, STR_LIT("alpha")));
  CHECK(str_ends_with(text, STR_LIT("bet")));
  CHECK(str_find_byte(STR_LIT("abca"), 'a') == 0);
  CHECK(str_rfind_byte(STR_LIT("abca"), 'a') == 3);
  CHECK(str_find_byte(STR_LIT("abca"), 'x') == SIGMA_NPOS);
  CHECK(str_eq(str_trim_ascii(STR_LIT(" \t hello \r\n")), STR_LIT("hello")));

  str_split_scalar_t split = str_split_scalar(STR_LIT("a::b:"), ':');
  const str_t expected[] = {STR_LIT("a"), STR_LIT(""), STR_LIT("b"),
                            STR_LIT("")};
  str_t part;
  for (usize index = 0; index < 4; ++index) {
    CHECK(str_split_scalar_next(&split, &part));
    CHECK(str_eq(part, expected[index]));
  }
  CHECK(!str_split_scalar_next(&split, &part));

  const u8 raw[] = {0, 1, 2, 3};
  bytes_t bytes = {.items = raw, .len = sizeof(raw)};
  CHECK(!bytes_is_empty(bytes));
  CHECK(bytes_is_empty((bytes_t){0}));
  CHECK(bytes_eq(bytes_sub(bytes, 1, 2),
                 ((bytes_t){.items = raw + 1, .len = 2})));
  CHECK(bytes_find(bytes, 2) == 2 && bytes_rfind(bytes, 0) == 0);
  CHECK(bytes_cmp((bytes_t){.items = (u8[]){0, 0xff}, .len = 2},
                  (bytes_t){.items = (u8[]){0, 0}, .len = 2}) > 0);
  CHECK(bytes_sub(bytes, 99, 99).len == 0);
  CHECK(bytes_sub((bytes_t){0}, 0, 1).items == NULL);
  u8 mutable_source[] = {4, 5, 6};
  bytes_mut_t mutable = bytes_mut_sub(
      (bytes_mut_t){.items = mutable_source, .len = sizeof(mutable_source)}, 1,
      1);
  mutable.items[0] = 9;
  CHECK(mutable_source[1] == 9);

  alloc_count = 0;
  free_count = 0;
  char *converted =
      str_to_cstr((str_t){.items = "a\0b", .len = 3}, test_allocator());
  CHECK(converted != NULL && memcmp(converted, "a\0b\0", 4) == 0);
  test_free(NULL, converted, 4, alignof(max_align_t));
  CHECK(alloc_count == 1 && free_count == 1);
  return true;
}

bool sigma_test_characters(void) {
  for (unsigned value = 0; value < 256; ++value) {
    u8 byte = (u8)value;
    bool lower = byte >= 'a' && byte <= 'z';
    bool upper = byte >= 'A' && byte <= 'Z';
    bool digit = byte >= '0' && byte <= '9';
    bool space = byte == ' ' || (byte >= '\t' && byte <= '\r');
    bool control = byte <= 0x1f || byte == 0x7f;
    bool printable = byte >= 0x20 && byte <= 0x7e;
    bool hexadecimal =
        digit || (byte >= 'A' && byte <= 'F') || (byte >= 'a' && byte <= 'f');
    CHECK(ascii_is_alpha(byte) == (lower || upper));
    CHECK(ascii_is_digit(byte) == digit);
    CHECK(ascii_is_alnum(byte) == (lower || upper || digit));
    CHECK(ascii_is_space(byte) == space);
    CHECK(ascii_is_cntrl(byte) == control);
    CHECK(ascii_is_print(byte) == printable);
    CHECK(ascii_is_lower(byte) == lower);
    CHECK(ascii_is_upper(byte) == upper);
    CHECK(ascii_is_xdigit(byte) == hexadecimal);
    CHECK(ascii_to_lower(byte) == (upper ? byte + 32U : byte));
    CHECK(ascii_to_upper(byte) == (lower ? byte - 32U : byte));
  }
  sigma_rune lambda = sigma_rune_from_u32(0x03bb);
  sigma_rune upper = sigma_rune_from_u32(0x039b);
  CHECK(sigma_rune_is_alpha(lambda) && sigma_rune_is_lower(lambda));
  CHECK(sigma_rune_is_upper(upper));
  CHECK(sigma_rune_to_lower(upper).value == lambda.value);
  CHECK(sigma_rune_to_upper(lambda).value == upper.value);
  CHECK(sigma_rune_is_digit(sigma_rune_from_u32(0x0665)));
  CHECK(sigma_rune_is_space(sigma_rune_from_u32(0x00a0)));
  CHECK(sigma_rune_is_print(sigma_rune_from_u32(0x1f642)));
  CHECK(sigma_rune_is_cntrl(sigma_rune_from_u32(0x000a)));
  CHECK(sigma_rune_is_xdigit(sigma_rune_from_u32(0xff21)));
  sigma_rune surrogate = sigma_rune_from_u32(0xd800);
  CHECK(!sigma_rune_is_alpha(surrogate));
  CHECK(sigma_rune_to_lower(surrogate).value == surrogate.value);
  return true;
}

bool sigma_test_collections_and_reader(void) {
  sigma_str_map map;
  CHECK(sigma_str_map_init(&map, test_allocator(), 0));
  CHECK(sigma_str_hash(STR_LIT("left")) != sigma_str_hash(STR_LIT("right")));
  CHECK(sigma_str_map_put(&map, STR_LIT("key"), STR_LIT("value")));
  CHECK(sigma_str_map_has(&map, STR_LIT("key")));
  str_t *found = sigma_str_map_get(&map, STR_LIT("key"));
  CHECK(found != NULL && str_eq(*found, STR_LIT("value")));
  sigma_str_map_deinit(&map);
  CHECK(map.cap == 0);

  sigma_str_map sized_map;
  CHECK(sigma_str_map_init(&sized_map, test_allocator(), 4));
  CHECK(sized_map.cap == 4);
  sigma_str_map_deinit(&sized_map);

  sigma_line_reader invalid_reader;
  sigma_line_reader_init(&invalid_reader, -1, test_allocator());
  CHECK(invalid_reader.fd == -1);
  sigma_line_reader_deinit(&invalid_reader);
  CHECK(invalid_reader.reached_eof);

  int fds[2];
  CHECK(pipe(fds) == 0);
  const char input[] = "first\nsecond\r\n";
  CHECK(write(fds[1], input, sizeof(input) - 1) ==
        (ssize_t)(sizeof(input) - 1));
  close(fds[1]);
  sigma_line_reader reader;
  sigma_line_reader_init(&reader, fds[0], test_allocator());
  sigma_line_result first = sigma_line_reader_next(&reader);
  CHECK(first.tag == sigma_line_ok && str_eq(first.line, STR_LIT("first")));
  sigma_line_result second = sigma_line_reader_next(&reader);
  CHECK(second.tag == sigma_line_ok && str_eq(second.line, STR_LIT("second")));
  sigma_line_result eof = sigma_line_reader_next(&reader);
  CHECK(eof.tag == sigma_line_eof);
  sigma_line_reader_deinit(&reader);
  close(fds[0]);

  alloc_count = 0;
  free_count = 0;
  u8 long_input[9001];
  memset(long_input, 'x', sizeof(long_input));
  long_input[sizeof(long_input) - 1] = '\n';
  CHECK(pipe(fds) == 0);
  CHECK(write(fds[1], long_input, sizeof(long_input)) ==
        (ssize_t)sizeof(long_input));
  close(fds[1]);
  sigma_line_reader_init(&reader, fds[0], test_allocator());
  sigma_line_result long_line = sigma_line_reader_next(&reader);
  CHECK(long_line.tag == sigma_line_ok &&
        long_line.line.len == sizeof(long_input) - 1);
  CHECK(reader.arena.blocks != NULL);
  sigma_line_reader_deinit(&reader);
  CHECK(reader.arena.blocks == NULL && alloc_count > 0 &&
        alloc_count == free_count);
  close(fds[0]);
  return true;
}

bool sigma_test_core(void) {
  return sigma_test_conversion_and_memory() && sigma_test_lists() &&
         sigma_test_strings_and_bytes() && sigma_test_characters() &&
         sigma_test_collections_and_reader();
}
