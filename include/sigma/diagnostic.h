#pragma once

#include <assert.h>

#include <sigma/meta.h>

/* Builds one structured compile-time diagnostic for compilers and tooling. */
#define SIGMA_DIAGNOSTIC(code, message, expression, help)                      \
  "\n\nerror[" code "]: " message "\n --> " __FILE__                           \
  ":" SIGMA_STRINGIFY(__LINE__) "\n  |\n  | `" #expression                     \
                                "`\n  |\n  = help: " help "\n"

/* Rejects an invalid compile-time contract with source and recovery context. */
#define SIGMA_STATIC_ASSERT(condition, code, message, expression, help)        \
  static_assert((condition), SIGMA_DIAGNOSTIC(code, message, expression, help))

/* Adds a declaration-context static assertion to an ordinary expression. */
#define SIGMA_REQUIRE_EXPR(condition, code, message, expression, help, result) \
  ((void)sizeof(struct {                                                       \
     SIGMA_STATIC_ASSERT(condition, code, message, expression, help);          \
     unsigned char sigma_checked__;                                            \
   }),                                                                         \
   (result))
