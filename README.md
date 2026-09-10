# sigma_libft

A C23 utility library for the Sigma ecosystem. It is built with Zig 0.16.0
and integrates with `sigma_malloc`.

The code deliberately uses C23 metaprogramming for typed ownership helpers,
generic traits, formatting dispatch, and compile-time diagnostics.

## where i can point ppl to show to so i never get hired for C

- [`include/sigma/meta.h`](include/sigma/meta.h) contains the bounded 0–64
  argument preprocessor mapper and the older recursive macro machinery.
- [`include/sigma/printf.h`](include/sigma/printf.h) contains typed formatting
  arguments, custom formatter vtables, `_Generic` dispatch, and printf wrappers.
- [`include/sigma/diagnostic.h`](include/sigma/diagnostic.h) builds the
  source-aware compile-time error messages used by public macros.
- [`include/sigma/ownership.h`](include/sigma/ownership.h) provides checked
  zero, move, take, pointer-move, swap, and forget operations.
- [`include/sigma/traits.h`](include/sigma/traits.h) dispatches character,
  clone, deinit, and replace traits through the type registries.
- [`include/sigma/vec.h`](include/sigma/vec.h) generates typed borrowed,
  trivial, and owning vectors while preserving move/clone contracts.
- [`src/io/printf/format/format.c`](src/io/printf/format/format.c) is the small
  runtime format loop and builtin/custom dispatcher.
- [`src/io/printf/format/integer.c`](src/io/printf/format/integer.c) implements
  integer rendering, prefixes, signs, precision, fill, width, and alignment.
- [`src/internal/cpu.h`](src/internal/cpu.h) and the modern memory sources route
  scalar, SSE2, and AVX2 implementations.
- [`tools/sigma-diagnostics`](tools/sigma-diagnostics) is the optional,
  compiler-independent renderer for structured Sigma diagnostics.

## Current features

- bounded `str_t`, `bytes_t`, mutable views, slicing, comparison, searching,
  splitting, and trimming
- allocator-backed owning `string_t` and owning string vectors
- generated typed vectors with explicit ownership operations
- a borrowed `str_t -> str_t` hash map
- exact-range memory copy, move, set, compare, and find operations with SIMD
  routing where implemented
- ASCII classification and Unicode 17 simple classification/case mapping
- buffered line reading with explicit EOF, I/O, and allocation result tags
- typed formatting, custom formatting vtables, fd/string/fixed-buffer writers,
  and stdout/fd/fixed/allocated printf wrappers
- stable compile-time error codes with an optional diagnostic renderer
- Zig-driven C builds and tests across Debug, ReleaseFast, and selectable SIMD
  routes

Every C source currently under `src/` is referenced by `build.zig`; the previous
printf implementation and empty compatibility headers were removed rather than
left as dead alternatives.

## Macro density

`mise run stats` recalculates this block from every `.c` and `.h` file under
`include/` and `src/`. Macro lines are `#define` directives and their continued
lines. Other preprocessor directives, blank lines, and comment-only lines are
excluded, so the comparison is macro implementation against runtime C and
declarations rather than raw file length.

<!-- sigma-code-stats:start -->

| Kind | Lines | Share |
| --- | ---: | ---: |
| C23 macros | 689 | 23.7% |
| Runtime C and declarations | 2223 | 76.3% |
| Total classified code | 2912 | 100.0% |

<!-- sigma-code-stats:end -->

this is perfectly fine 😭 its not enough jork

## Build and test

The repository uses `mise` to pin tools and expose the usual commands:

```sh
mise run dev          # Debug build
mise run build        # ReleaseFast build
mise run test         # complete Zig-driven C test suite
mise run test:simd    # scalar, SSE2, AVX2, and automatic SIMD routing
mise run diagnostics  # show and verify intentional compile-time errors
mise run stats        # refresh README macro-to-C code statistics
mise run format       # format public headers and C sources
mise run clean        # remove generated build files
```

The equivalent diagnostic command is:

```sh
zig build diagnostics
```

The renderer is a standalone Go compiler wrapper. Build it with
`mise run build:diagnostics`, then use it with any build system:

```sh
zig-out/bin/sigma-diagnostics -- cc -std=c23 -Iinclude \
  -I../sigma_malloc/include -c your_file.c
```

Without the wrapper, the compiler still emits the complete assertion message.

## Formatting

The public formatting API is in `include/sigma/printf.h`. It currently provides:

- `sigma_writer_fmt` for runtime `str_t` format strings
- `sigma_printf` and `sigma_print` for stdout
- `sigma_fprintf` for file descriptors
- `sigma_snprintf` for fixed NUL-terminated buffers
- `sigma_asprintf` for allocator-backed owning `string_t` results
- builtin formatting for integers, booleans, characters, runes, strings,
  bytes, and pointers
- width, precision, fill, alignment, sign, alternate-prefix, zero-padding,
  decimal, hexadecimal, binary, octal, pointer, and debug presentations
- borrowed custom formatters through `sigma_formatter_vtable`

Custom types opt into automatic `_Generic` formatting by defining
`SIGMA_CUSTOM_FORMAT_TYPES` before including `sigma/printf.h`:

```c
typedef struct {
  i32 x;
  i32 y;
} point_t;

struct sigma_formatter_vtable;
extern const struct sigma_formatter_vtable sigma_point_formatter;

#define SIGMA_CUSTOM_FORMAT_TYPES(X) \
  X(POINT, point_t, sigma_point_formatter)

#include <sigma/printf.h>
```

Custom values are borrowed for the formatting call. Their formatters must not
mutate, move, free, or otherwise take ownership of the value.

Floating-point values have argument storage reserved but rendering is not
implemented yet. Literal brace escaping is also not implemented yet.

## Compile-time diagnostics

Public generic and ownership macros use C23 `static_assert` with stable error
codes, source locations, the offending expression, and recovery help. Compilers
render these assertions directly; the optional Go wrapper can restyle them.
Compile-time contracts are collected in `tests/compile_contracts.c`; intentional
failures used by the diagnostic runner live in
`tests/compile_fail/diagnostics.c`.

## Unicode database

`src/unicode/unicode.bin` is generated from the pinned Unicode 17.0.0
`UnicodeData.txt`, `DerivedCoreProperties.txt`, and `PropList.txt` files:

```sh
zig cc -std=c23 -Wall -Wextra -Wpedantic -pedantic-errors \
  tools/unicode_gen.c -o /tmp/sigma-unicode-gen
/tmp/sigma-unicode-gen \
  /path/to/UnicodeData.txt \
  /path/to/DerivedCoreProperties.txt \
  /path/to/PropList.txt \
  src/unicode/unicode.bin
```

The generated database derives from the Unicode Character Database and is
covered by the [Unicode Terms of Use](https://www.unicode.org/terms_of_use.html).

## Current TODOs

- move debug moved-from poisoning into `sigma_rt`
- add a reusable `Result(T, E)` facility instead of defining result tags per API
- make the hash map generator generic over key/value/hash/equality contracts
- extend SIMD routing beyond the current memory operations and measure each path
- integrate runtime services from `sigma_rt` instead of growing local wrappers
- deepen `sigma_malloc` integration, including allocation diagnostics and
  consistent failure propagation through higher-level containers
- add automatic custom formatting registrations for production collection types
- implement floating-point rendering and literal brace escaping
- add a growable formatting API that can report allocation/format errors without
  an out-parameter-only boolean contract
- add Unicode special casing through a string-producing API
- add UTF-8 iteration before normalization and grapheme iteration
- add syscall wrappers
- remove remaining hidden allocations
- finish and consistently apply Sigma annotations
- add more generated compile-fail fixtures as public macro contracts grow
- measure before adding further low-level optimizations
