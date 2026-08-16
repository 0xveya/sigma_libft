#include <sigma/hash_map.h>
#include <sigma/libft.h>

/* sigma:begin
name: libft.str.from_cstr
provides: str.from_cstr
deps: str.len
externals:
kind: function
*/
str_t sigma_str_from_cstr(const char *text) {
  return (str_t){.items = (char *)text, .len = ft_strlen(text)};
}
/* sigma:end */

/* sigma:begin
name: libft.str.hash
provides: str.hash
deps:
externals:
kind: function
*/
u64 sigma_str_hash(str_t string) {
  u64 hash = 5381;
  for (usize i = 0; i < string.len; i++)
    hash = ((hash << 5) + hash) + (u8)string.items[i];
  return hash;
}
/* sigma:end */

static bool str_eq(str_t left, str_t right) {
  return left.len == right.len &&
         ft_memcmp(left.items, right.items, left.len) == 0;
}

static sigma_map_entry *find_entry(sigma_map_entry *entries, usize capacity,
                                   str_t key) {
  usize index = sigma_str_hash(key) % capacity;
  for (;;) {
    sigma_map_entry *entry = &entries[index];
    if (!entry->occupied || str_eq(entry->key, key))
      return entry;
    index = (index + 1) % capacity;
  }
}

static sigma_map_entry *alloc_entries(sigma_str_map *map, usize capacity) {
  if (map->allocator.vtable == nullptr ||
      map->allocator.vtable->alloc == nullptr ||
      capacity > SIZE_MAX / sizeof(sigma_map_entry))
    return nullptr;
  sigma_map_entry *entries = map->allocator.vtable->alloc(
      map->allocator.ctx, capacity * sizeof(*entries),
      _Alignof(sigma_map_entry));
  if (entries != nullptr)
    ft_memset(entries, 0, capacity * sizeof(*entries));
  return entries;
}

static void free_entries(sigma_str_map *map) {
  if (map->entries != nullptr && map->allocator.vtable != nullptr &&
      map->allocator.vtable->free != nullptr)
    map->allocator.vtable->free(map->allocator.ctx, map->entries,
                                map->cap * sizeof(*map->entries),
                                _Alignof(sigma_map_entry));
}

static bool grow(sigma_str_map *map) {
  usize capacity = map->cap == 0 ? 8 : map->cap * 2;
  if (capacity < map->cap)
    return false;
  sigma_map_entry *entries = alloc_entries(map, capacity);
  if (entries == nullptr)
    return false;
  for (usize i = 0; i < map->cap; i++)
    if (map->entries[i].occupied)
      *find_entry(entries, capacity, map->entries[i].key) = map->entries[i];
  free_entries(map);
  map->entries = entries;
  map->cap = capacity;
  return true;
}

/* sigma:begin
name: libft.str.map
provides: str.map
deps: mem.alloc, mem.free, mem.compare, str.hash
externals:
kind: function
*/
bool sigma_str_map_init(sigma_str_map *map, allocator_t allocator,
                        usize capacity) {
  if (map == nullptr)
    return false;
  *map = (sigma_str_map){.allocator = allocator};
  if (capacity == 0)
    return true;
  map->entries = alloc_entries(map, capacity);
  if (map->entries == nullptr)
    return false;
  map->cap = capacity;
  return true;
}

void sigma_str_map_deinit(sigma_str_map *map) {
  free_entries(map);
  *map = (sigma_str_map){};
}

str_t *sigma_str_map_get(sigma_str_map *map, str_t key) {
  if (map == nullptr || map->cap == 0)
    return nullptr;
  usize index = sigma_str_hash(key) % map->cap;
  for (usize tries = 0; tries < map->cap; tries++) {
    sigma_map_entry *entry = &map->entries[index];
    if (!entry->occupied)
      return nullptr;
    if (str_eq(entry->key, key))
      return &entry->value;
    index = (index + 1) % map->cap;
  }
  return nullptr;
}

bool sigma_str_map_has(sigma_str_map *map, str_t key) {
  return sigma_str_map_get(map, key) != nullptr;
}

bool sigma_str_map_put(sigma_str_map *map, str_t key, str_t value) {
  if (map == nullptr)
    return false;
  if (map->cap != 0) {
    sigma_map_entry *entry = find_entry(map->entries, map->cap, key);
    if (entry->occupied) {
      entry->value = value;
      return true;
    }
  }
  if (map->cap == 0 || (map->len + 1) * 100 > map->cap * 67)
    if (!grow(map))
      return false;
  sigma_map_entry *entry = find_entry(map->entries, map->cap, key);
  *entry = (sigma_map_entry){.key = key, .value = value, .occupied = true};
  map->len++;
  return true;
}
/* sigma:end */
