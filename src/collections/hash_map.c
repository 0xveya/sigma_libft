#include <sigma/hash_map.h>
#include <sigma/libft.h>

/** Performs the sigma str hash operation. */
u64 sigma_str_hash(str_t string) {
  u64 hash = 5381;
  for (usize i = 0; i < string.len; i++)
    hash = ((hash << 5) + hash) + (u8)string.items[i];
  return hash;
}

static sigma_map_entry *find_entry(sigma_map_entry *entries, usize capacity,
                                   str_t key) {
  usize index = sigma_str_hash(key) % capacity;
  for (usize tries = 0; tries < capacity; tries++) {
    sigma_map_entry *entry = &entries[index];
    if (!entry->occupied || str_eq(entry->key, key))
      return entry;
    index = (index + 1) % capacity;
  }
  return nullptr;
}

RESULT(map_entries_result, sigma_map_entry *, sigma_map_error);

static map_entries_result alloc_entries(sigma_str_map *map, usize capacity) {
  if (capacity > SIZE_MAX / sizeof(sigma_map_entry))
    return ERR(map_entries_result, sigma_map_capacity_overflow);
  if (map->allocator.vtable == nullptr ||
      map->allocator.vtable->alloc == nullptr)
    return ERR(map_entries_result, sigma_map_out_of_memory);
  sigma_map_entry *entries = map->allocator.vtable->alloc(
      map->allocator.ctx, capacity * sizeof(*entries),
      _Alignof(sigma_map_entry));
  if (entries == nullptr)
    return ERR(map_entries_result, sigma_map_out_of_memory);
  ft_memset(entries, 0, capacity * sizeof(*entries));
  return OK(map_entries_result, entries);
}

static void free_entries(sigma_str_map *map) {
  if (map->entries != nullptr && map->allocator.vtable != nullptr &&
      map->allocator.vtable->free != nullptr)
    map->allocator.vtable->free(map->allocator.ctx, map->entries,
                                map->cap * sizeof(*map->entries),
                                _Alignof(sigma_map_entry));
}

static sigma_map_status grow(sigma_str_map *map) {
  usize capacity = map->cap == 0 ? 8 : map->cap * 2;
  if (capacity < map->cap)
    return ERR(sigma_map_status, sigma_map_capacity_overflow);
  sigma_map_entry *entries =
      TRY(sigma_map_status, alloc_entries(map, capacity));
  for (usize i = 0; i < map->cap; i++)
    if (map->entries[i].occupied)
      *find_entry(entries, capacity, map->entries[i].key) = map->entries[i];
  free_entries(map);
  map->entries = entries;
  map->cap = capacity;
  return OK(sigma_map_status, sigma_unit);
}

/** Performs the sigma str map init operation. */
sigma_map_status sigma_str_map_init(sigma_str_map *map, allocator_t allocator,
                                    usize capacity) {
  if (map == nullptr)
    return ERR(sigma_map_status, sigma_map_invalid_argument);
  *map = (sigma_str_map){.allocator = allocator};
  if (capacity == 0)
    return OK(sigma_map_status, sigma_unit);
  map->entries = TRY(sigma_map_status, alloc_entries(map, capacity));
  map->cap = capacity;
  return OK(sigma_map_status, sigma_unit);
}

/** Performs the sigma str map deinit operation. */
void sigma_str_map_deinit(sigma_str_map *map) {
  free_entries(map);
  *map = (sigma_str_map){};
}

/** Performs the sigma str map get operation. */
sigma_map_value sigma_str_map_get(sigma_str_map *map, str_t key) {
  if (map == nullptr || map->cap == 0)
    return NONE(sigma_map_value);
  sigma_map_entry *entry = find_entry(map->entries, map->cap, key);
  if (entry == nullptr || !entry->occupied)
    return NONE(sigma_map_value);
  return SOME(sigma_map_value, &entry->value);
}

/** Performs the sigma str map has operation. */
bool sigma_str_map_has(sigma_str_map *map, str_t key) {
  return sigma_str_map_get(map, key).some;
}

/** Performs the sigma str map put operation. */
sigma_map_status sigma_str_map_put(sigma_str_map *map, str_t key, str_t value) {
  if (map == nullptr)
    return ERR(sigma_map_status, sigma_map_invalid_argument);
  if (map->cap != 0) {
    sigma_map_entry *entry = find_entry(map->entries, map->cap, key);
    if (entry != nullptr && entry->occupied) {
      entry->value = value;
      return OK(sigma_map_status, sigma_unit);
    }
  }
  if (map->len == map->cap || (map->len + 1) * 100 > map->cap * 67)
    (void)TRY(sigma_map_status, grow(map));
  sigma_map_entry *entry = find_entry(map->entries, map->cap, key);
  *entry = (sigma_map_entry){.key = key, .value = value, .occupied = true};
  map->len++;
  return OK(sigma_map_status, sigma_unit);
}
