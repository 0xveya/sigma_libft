#pragma once

#include <sigma/meta.h>

/* Declare named types once and reuse them in function signatures. */
#define RESULT(Name, Value, Error)                                             \
  typedef struct Name {                                                        \
    bool ok;                                                                   \
    union {                                                                    \
      Value value;                                                             \
      Error error;                                                             \
    };                                                                         \
  } Name

#define OPTIONAL(Name, Value)                                                  \
  typedef struct Name {                                                        \
    bool some;                                                                 \
    Value value;                                                               \
  } Name

#define OK(Type, ...) ((Type){.ok = true, .value = (__VA_ARGS__)})
#define ERR(Type, ...) ((Type){.error = (__VA_ARGS__)})
#define SOME(Type, ...) ((Type){.some = true, .value = (__VA_ARGS__)})
#define NONE(Type) ((Type){0})

typedef enum { sigma_unit } sigma_unit_t;

/* Evaluates once. Returns an error as Type or yields the success value.
 * Type is the enclosing function's result type and must share the error type.
 * Uses a GNU statement expression.
 */
#define TRY(Type, ...)                                                         \
  SIGMA_TRY(Type, SIGMA_CAT(sigma_try_result_, __LINE__), __VA_ARGS__)
#define SIGMA_TRY(Type, temporary, ...)                                        \
  __extension__({                                                              \
    auto temporary = (__VA_ARGS__);                                            \
    static_assert(_Generic(temporary.error,                                    \
                      typeof(((Type){0}).error): true,                         \
                      default: false),                                         \
                  "TRY requires the same error type");                         \
    if (!temporary.ok)                                                         \
      return ERR(Type, temporary.error);                                       \
    temporary.value;                                                           \
  })
