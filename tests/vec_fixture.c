#include <sigma/ownership.h>
#include <sigma/vec.h>

#include <stdlib.h>

typedef struct {
  bool fail_alloc;
  usize backing_allocs;
  usize backing_frees;
} test_allocator_ctx_t;

typedef struct {
  int *ptr;
  usize *deinit_count;
} owned_int_t;

static void *test_alloc(void *raw_ctx, usize size, usize alignment) {
  (void)alignment;
  test_allocator_ctx_t *ctx = raw_ctx;

  if (ctx->fail_alloc)
    return NULL;

  void *ptr = malloc(size);
  if (ptr != NULL)
    ++ctx->backing_allocs;

  return ptr;
}

static void test_free(void *raw_ctx, void *ptr, usize size, usize alignment) {
  (void)size;
  (void)alignment;
  test_allocator_ctx_t *ctx = raw_ctx;

  ++ctx->backing_frees;
  free(ptr);
}

static allocator_t test_allocator(test_allocator_ctx_t *ctx) {
  static const allocator_vtable_t vtable = {
      .alloc = test_alloc,
      .free = test_free,
  };

  return (allocator_t){
      .ctx = ctx,
      .vtable = &vtable,
  };
}

static owned_int_t owned_int_init(int value, usize *deinit_count) {
  int *ptr = malloc(sizeof(*ptr));
  if (ptr == NULL)
    return (owned_int_t){0};

  *ptr = value;
  return (owned_int_t){
      .ptr = ptr,
      .deinit_count = deinit_count,
  };
}

static void owned_int_deinit(owned_int_t *value) {
  if (value->ptr != NULL) {
    free(value->ptr);
    ++*value->deinit_count;
  }

  *value = (owned_int_t){0};
}

static bool owned_int_clone(owned_int_t *out, const owned_int_t *source) {
  *out = owned_int_init(*source->ptr, source->deinit_count);
  return out->ptr != NULL;
}

SIGMA_VEC_DEFINE_TRIVIAL(i32, test_i32_vec)
SIGMA_VEC_DEFINE_OWNED(owned_int_t, test_owned_vec, owned_int_clone,
                       owned_int_deinit)

static bool test_trivial_vec(allocator_t allocator) {
  test_i32_vec source = test_i32_vec_init(allocator);

  for (i32 value = 0; value < 9; ++value) {
    if (!test_i32_vec_push(&source, value))
      return false;
  }

  test_i32_vec_view_t view = test_i32_vec_view(&source);
  test_i32_vec_view_mut_t view_mut = test_i32_vec_view_mut(&source);
  if (view.len != 9 || view.items[8] != 8 || view_mut.len != 9)
    return false;

  view_mut.items[0] = 42;
  if (*test_i32_vec_get(&source, 0) != 42 ||
      test_i32_vec_get_const(&source, 9) != NULL)
    return false;

  i32 popped = 0;
  if (!test_i32_vec_pop(&source, &popped) || popped != 8)
    return false;

  test_i32_vec moved = test_i32_vec_take(&source);
  if (source.items != NULL || source.len != 0 || source.cap != 0 ||
      source.allocator.vtable != NULL || moved.len != 8)
    return false;

  test_i32_vec_clear(&moved);
  if (!test_i32_vec_is_empty(&moved))
    return false;

  test_i32_vec_deinit(&moved);
  return moved.items == NULL && moved.allocator.vtable == NULL;
}

static bool test_take_macro(void) {
  owned_int_t source = owned_int_init(42, &(usize){0});
  if (source.ptr == NULL)
    return false;

  owned_int_t destination = {0};
  SIGMA_TAKE(destination, source);

  if (source.ptr != NULL || source.deinit_count != NULL ||
      destination.ptr == NULL || *destination.ptr != 42) {
    owned_int_deinit(&destination);
    return false;
  }

  owned_int_deinit(&destination);
  return true;
}

static bool test_owned_vec_contract(allocator_t allocator,
                                    test_allocator_ctx_t *ctx) {
  usize deinit_count = 0;
  test_owned_vec vec = test_owned_vec_init(allocator);
  owned_int_t first = owned_int_init(10, &deinit_count);
  owned_int_t second = owned_int_init(20, &deinit_count);

  if (first.ptr == NULL || second.ptr == NULL)
    return false;

  ctx->fail_alloc = true;
  if (test_owned_vec_push_take(&vec, &first) || first.ptr == NULL ||
      vec.items != NULL || vec.len != 0) {
    owned_int_deinit(&first);
    owned_int_deinit(&second);
    return false;
  }

  ctx->fail_alloc = false;
  if (!test_owned_vec_push_take(&vec, &first) || first.ptr != NULL ||
      !test_owned_vec_push_take(&vec, &second) || second.ptr != NULL)
    return false;

  owned_int_t popped = {0};
  if (!test_owned_vec_pop_take(&vec, &popped) || popped.ptr == NULL ||
      *popped.ptr != 20 || vec.items[vec.len].ptr != NULL)
    return false;

  owned_int_deinit(&popped);
  if (deinit_count != 1)
    return false;

  test_owned_vec moved = test_owned_vec_take(&vec);
  if (vec.items != NULL || vec.len != 0 || vec.cap != 0 ||
      vec.allocator.vtable != NULL || moved.len != 1)
    return false;

  test_owned_vec_deinit(&moved);
  return deinit_count == 2 && moved.items == NULL && moved.len == 0 &&
         moved.cap == 0 && moved.allocator.vtable == NULL;
}

bool sigma_test_vec_codegen(void) {
  test_allocator_ctx_t ctx = {0};
  allocator_t allocator = test_allocator(&ctx);

  if (!test_trivial_vec(allocator) || !test_take_macro() ||
      !test_owned_vec_contract(allocator, &ctx))
    return false;

  return ctx.backing_allocs == ctx.backing_frees;
}

#if defined(SIGMA_VEC_FIXTURE_MAIN)
int main(void) {
  return sigma_test_vec_codegen() ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif
