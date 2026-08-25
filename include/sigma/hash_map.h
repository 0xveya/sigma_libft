#pragma once

#include <allocator_vtable.h>
#include <sigma/attributes.h>
#include <sigma/str.h>
#include <stdbool.h>

typedef struct sigma_map_entry {
  str_t key;
  str_t value;
  bool occupied;
} sigma_map_entry;

typedef struct sigma_str_map {
  sigma_map_entry *entries;
  usize len;
  usize cap;
  allocator_t allocator;
} sigma_str_map;

/* Hashes the bounded bytes in string. */
u64 sigma_str_hash(str_t string);
/* Initializes map with at least capacity slots using allocator. */
SIGMA_NODISCARD bool sigma_str_map_init(sigma_str_map *map,
                                        allocator_t allocator, usize capacity);
/* Releases map storage and resets map to {0}. */
void sigma_str_map_deinit(sigma_str_map *map);
/* Returns the borrowed value for key, or NULL when key is absent. */
SIGMA_NODISCARD str_t *sigma_str_map_get(sigma_str_map *map, str_t key);
/* Returns true when map contains key. */
bool sigma_str_map_has(sigma_str_map *map, str_t key);
/* Inserts or replaces borrowed key and value views. */
SIGMA_NODISCARD bool sigma_str_map_put(sigma_str_map *map, str_t key,
                                       str_t value);
