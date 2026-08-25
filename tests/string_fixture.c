#include <sigma/ownership_registry.h>

#include <stdlib.h>

typedef struct {
  bool fail_alloc;
  usize allocs;
  usize frees;
} string_test_allocator_ctx_t;

static void *string_test_alloc(void *raw_ctx, usize size, usize alignment) {
  (void)alignment;
  string_test_allocator_ctx_t *ctx = raw_ctx;

  if (ctx->fail_alloc)
    return NULL;

  void *ptr = malloc(size);
  if (ptr != NULL)
    ++ctx->allocs;

  return ptr;
}

static void string_test_free(void *raw_ctx, void *ptr, usize size,
                             usize alignment) {
  (void)size;
  (void)alignment;
  string_test_allocator_ctx_t *ctx = raw_ctx;

  ++ctx->frees;
  free(ptr);
}

static allocator_t string_test_allocator(string_test_allocator_ctx_t *ctx) {
  static const allocator_vtable_t vtable = {
      .alloc = string_test_alloc,
      .free = string_test_free,
  };

  return (allocator_t){
      .ctx = ctx,
      .vtable = &vtable,
  };
}

static bool string_test_basics(allocator_t allocator,
                               string_test_allocator_ctx_t *ctx) {
  string_t string = string_init(allocator);

  if (!string_append(&string, STR_LIT("hello")) ||
      !string_append_char(&string, ' ') ||
      !string_append(&string, STR_LIT("sigma")) ||
      !str_eq(string_view(&string), STR_LIT("hello sigma")))
    return false;

  usize cap = string.cap;
  string_view_mut(&string).items[0] = 'H';
  string_clear(&string);
  if (string.len != 0 || string.cap != cap ||
      !string_append(&string, STR_LIT("Hi")))
    return false;

  ctx->fail_alloc = true;
  char *items = string.items;
  usize len = string.len;
  if (string_reserve(&string, cap + 1) || string.items != items ||
      string.len != len || string.cap != cap)
    return false;
  ctx->fail_alloc = false;

  string_t moved = string_take(&string);
  if (string.items != NULL || string.len != 0 || string.cap != 0 ||
      string.allocator.vtable != NULL ||
      !str_eq(string_view(&moved), STR_LIT("Hi")))
    return false;

  string_deinit(&string);
  string_deinit(&moved);
  return true;
}

static bool string_test_clone(allocator_t allocator,
                              string_test_allocator_ctx_t *ctx) {
  string_t clone = {0};
  if (!string_clone(allocator, STR_LIT("sigma"), &clone) ||
      !str_eq(string_view(&clone), STR_LIT("sigma")))
    return false;

  string_t untouched = string_init(allocator);
  ctx->fail_alloc = true;
  if (string_clone(allocator, STR_LIT("failure"), &untouched) ||
      untouched.items != NULL || untouched.len != 0 || untouched.cap != 0 ||
      untouched.allocator.vtable != allocator.vtable)
    return false;
  ctx->fail_alloc = false;

  string_deinit(&clone);
  string_deinit(&untouched);
  return true;
}

static bool string_test_vec(allocator_t allocator) {
  string_vec strings = string_vec_init(allocator);
  string_t first = {0};
  string_t second = {0};
  string_t third = {0};

  if (!string_clone(allocator, STR_LIT("foo"), &first) ||
      !string_clone(allocator, STR_LIT("bar"), &second) ||
      !string_clone(allocator, STR_LIT("baz"), &third))
    return false;

  if (!string_vec_push_take(&strings, &first) || first.items != NULL ||
      !string_vec_push_take(&strings, &second) || second.items != NULL ||
      !string_vec_push_take(&strings, &third) || third.items != NULL ||
      strings.len != 3 ||
      !str_eq(string_view(&strings.items[0]), STR_LIT("foo")) ||
      !str_eq(string_view(&strings.items[1]), STR_LIT("bar")) ||
      !str_eq(string_view(&strings.items[2]), STR_LIT("baz")))
    return false;

  string_t popped = {0};
  if (!string_vec_pop_take(&strings, &popped) || strings.len != 2 ||
      !str_eq(string_view(&popped), STR_LIT("baz")) ||
      strings.items[strings.len].items != NULL)
    return false;

  string_deinit(&popped);
  string_vec_deinit(&strings);
  return strings.items == NULL && strings.len == 0 && strings.cap == 0 &&
         strings.allocator.vtable == NULL;
}

static bool string_test_registry(allocator_t allocator) {
  string_t destination = {0};
  string_t source = {0};

  if (!string_clone(allocator, STR_LIT("old"), &destination) ||
      !string_clone(allocator, STR_LIT("new"), &source))
    return false;

  SIGMA_REPLACE(destination, source);
  if (source.items != NULL ||
      !str_eq(string_view(&destination), STR_LIT("new")))
    return false;

  string_vec strings = string_vec_init(allocator);
  if (!string_vec_push_take(&strings, &destination) ||
      destination.items != NULL)
    return false;

  SIGMA_DROP(strings);
  return strings.items == NULL && strings.len == 0 && strings.cap == 0 &&
         strings.allocator.vtable == NULL;
}

bool sigma_test_string_ownership(void) {
  string_test_allocator_ctx_t ctx = {0};
  allocator_t allocator = string_test_allocator(&ctx);

  if (!string_test_basics(allocator, &ctx) ||
      !string_test_clone(allocator, &ctx) || !string_test_vec(allocator) ||
      !string_test_registry(allocator))
    return false;

  return ctx.allocs == ctx.frees;
}
