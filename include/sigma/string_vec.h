#pragma once

#include <sigma/string.h>
#include <sigma/vec.h>

/*
 * Owns string_t elements and its backing allocation.
 *
 * push_take and pop_take transfer ownership through their pointer argument.
 * clear and deinit call string_deinit for every element still owned by the
 * vector. The complete generated API and its contracts are documented by
 * SIGMA_VEC_DEFINE_OWNED in <sigma/vec.h>.
 */
SIGMA_VEC_DEFINE_OWNED(string_t, string_vec, string_clone, string_deinit)
