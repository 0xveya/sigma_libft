#include <sigma/qol.h>

typedef struct {
  i32 x;
  i32 y;
} point_t;

struct sigma_formatter_vtable;
extern const struct sigma_formatter_vtable point_formatter;

#define SIGMA_CUSTOM_FORMAT_TYPES(X) X(POINT, point_t, point_formatter)

#include <sigma/printf.h>

#include "../src/io/printf/format/format_internal.h"

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

static void *format_test_alloc(void *ctx, usize size, usize alignment) {
  (void)ctx;
  (void)alignment;
  return malloc(size);
}

static void format_test_free(void *ctx, void *ptr, usize size,
                             usize alignment) {
  (void)ctx;
  (void)size;
  (void)alignment;
  free(ptr);
}

static allocator_t format_test_allocator(void) {
  static const allocator_vtable_t vtable = {
      .alloc = format_test_alloc,
      .free = format_test_free,
  };

  return (allocator_t){.vtable = &vtable};
}

static bool point_format(sigma_writer writer, const void *value,
                         const sigma_fmt_spec *spec) {
  const point_t *point = value;

  switch (spec->presentation) {
  case SIGMA_FMT_PRESENT_DEFAULT:
    return sigma_writer_fmt(writer, STR_LIT("({}, {})"), point->x, point->y);
  case SIGMA_FMT_PRESENT_DEBUG:
    return sigma_writer_str(writer, STR_LIT("point_t { x: ")) &&
           sigma_writer_fmt(writer, STR_LIT("{}, y: {}"), point->x, point->y) &&
           sigma_writer_str(writer, STR_LIT(" }"));
  default:
    return false;
  }
}

const sigma_formatter_vtable point_formatter = {
    .format = point_format,
};

static bool format_test_case(str_t format, const sigma_fmt_arg_t *args,
                             usize arg_count, str_t expected) {
  string_t output = string_init(format_test_allocator());
  sigma_string_writer sink = sigma_string_writer_init(&output);
  sigma_writer writer = sigma_string_writer_as_writer(&sink);
  bool ok = sigma_format_args(writer, format, args, arg_count);
  bool matches = ok && str_eq(string_view(&output), expected);

  string_deinit(&output);
  return matches;
}

static bool format_parse_fails(str_t input) {
  sigma_fmt_field field;

  return sigma_fmt_parse_field(input, &field) == SIGMA_FMT_PARSE_ERROR;
}

static bool format_parse_has_presentation(str_t input,
                                          sigma_fmt_presentation presentation) {
  sigma_fmt_field field;

  return sigma_fmt_parse_field(input, &field) == SIGMA_FMT_PARSE_OK &&
         field.spec.presentation == presentation;
}

bool sigma_test_format_dispatch(void) {
  sigma_fmt_arg_t integer = sigma_fmt_arg((i32)-42);
  sigma_fmt_arg_t size = sigma_fmt_arg((usize)42);
  sigma_fmt_arg_t wide = sigma_fmt_arg((long long)-42000000000LL);
  sigma_fmt_arg_t floating = sigma_fmt_arg((f32)4.25F);
  sigma_fmt_arg_t text = sigma_fmt_arg(STR_LIT("sigma"));
  u8 storage[] = {0xde, 0xad, 0xbe, 0xef};
  bytes_t bytes = {.items = storage, .len = sizeof(storage)};
  sigma_fmt_arg_t data = sigma_fmt_arg(bytes);
  sigma_rune rune = sigma_rune_from_u32(0x03bb);
  sigma_fmt_arg_t codepoint = sigma_fmt_arg(rune);

  return integer.kind == SIGMA_FMT_I64 && integer.value.signed_integer == -42 &&
         size.kind == SIGMA_FMT_U64 && size.value.unsigned_integer == 42 &&
         wide.kind == SIGMA_FMT_I64 &&
         wide.value.signed_integer == -42000000000LL &&
         floating.kind == SIGMA_FMT_F64 && floating.value.floating == 4.25 &&
         text.kind == SIGMA_FMT_STR &&
         text.value.str.items == STR_LIT("sigma").items &&
         text.value.str.len == 5 && data.kind == SIGMA_FMT_BYTES &&
         data.value.bytes.items == storage && data.value.bytes.len == 4 &&
         codepoint.kind == SIGMA_FMT_RUNE &&
         codepoint.value.rune.value == rune.value;
}

bool sigma_test_format_runtime(void) {
  sigma_fmt_arg_t basic[] = {
      sigma_fmt_arg(42),
      sigma_fmt_arg(STR_LIT("sigma")),
  };
  sigma_fmt_arg_t limits[] = {
      sigma_fmt_arg(-42),         sigma_fmt_arg(0),
      sigma_fmt_arg(UINT64_MAX),  sigma_fmt_arg((i64)INT64_MIN),
      sigma_fmt_arg(STR_LIT("")),
  };
  u8 raw[] = {0xde, 0xad, 0xbe, 0xef};
  bytes_t bytes = {.items = raw, .len = sizeof(raw)};
  sigma_fmt_arg_t integer[] = {sigma_fmt_arg(255)};
  sigma_fmt_arg_t text[] = {sigma_fmt_arg(STR_LIT("hello"))};
  sigma_fmt_arg_t data[] = {sigma_fmt_arg(bytes)};
  sigma_fmt_arg_t scalars[] = {
      sigma_fmt_arg(true),
      sigma_fmt_arg((char)'!'),
      sigma_fmt_arg(sigma_rune_from_u32(0x03bb)),
      sigma_fmt_arg((const void *)NULL),
  };
  sigma_fmt_arg_t floating[] = {sigma_fmt_arg(4.25)};

  if (!format_test_case(STR_LIT("number={} name={}"), basic, 2,
                        STR_LIT("number=42 name=sigma")) ||
      !format_test_case(STR_LIT("{} {} {} {}{}"), limits, 5,
                        STR_LIT("-42 0 18446744073709551615 "
                                "-9223372036854775808")) ||
      !format_test_case(STR_LIT("|{:8}|"), text, 1, STR_LIT("|hello   |")) ||
      !format_test_case(STR_LIT("|{:>8}|"), text, 1, STR_LIT("|   hello|")) ||
      !format_test_case(STR_LIT("|{:^9}|"), text, 1, STR_LIT("|  hello  |")) ||
      !format_test_case(STR_LIT("|{:*^9}|"), text, 1, STR_LIT("|**hello**|")) ||
      !format_test_case(STR_LIT("|{:.3}|"), text, 1, STR_LIT("|hel|")) ||
      !format_test_case(STR_LIT("{:x} {:X} {:b} {:o} {:#x} {:08x}"),
                        (sigma_fmt_arg_t[]){integer[0], integer[0], integer[0],
                                            integer[0], integer[0], integer[0]},
                        6, STR_LIT("ff FF 11111111 377 0xff 000000ff")) ||
      !format_test_case(STR_LIT("|{:<6}|{:^7}|{:*>6}|"),
                        (sigma_fmt_arg_t[]){sigma_fmt_arg(42),
                                            sigma_fmt_arg(42),
                                            sigma_fmt_arg(42)},
                        3, STR_LIT("|42    |  42   |****42|")) ||
      !format_test_case(
          STR_LIT("{:+d}|{: d}|{:#b}|{:#o}|{:#X}|{:#08x}|{:.5d}|{:06d}"),
          (sigma_fmt_arg_t[]){sigma_fmt_arg(42), sigma_fmt_arg(42),
                              sigma_fmt_arg(42), sigma_fmt_arg(42),
                              sigma_fmt_arg(42), sigma_fmt_arg(42),
                              sigma_fmt_arg(42), sigma_fmt_arg(-42)},
          8, STR_LIT("+42| 42|0b101010|0o52|0X2A|0x00002a|00042|-00042")))
    return false;

  string_t output = string_init(format_test_allocator());
  sigma_string_writer output_sink = sigma_string_writer_init(&output);
  sigma_writer output_writer = sigma_string_writer_as_writer(&output_sink);
  if (!sigma_format_args(
          output_writer, STR_LIT("{} {:?} {:x} {:X} {:b}"),
          (sigma_fmt_arg_t[]){data[0], data[0], data[0], data[0], data[0]},
          5)) {
    string_deinit(&output);
    return false;
  }
  str_t rendered = string_view(&output);
  str_t suffix = STR_LIT(" [0xde, 0xad, 0xbe, 0xef] deadbeef DEADBEEF "
                         "11011110101011011011111011101111");
  bool bytes_ok =
      rendered.len == sizeof(raw) + suffix.len &&
      bytes_eq(str_bytes((str_t){.items = rendered.items, .len = sizeof(raw)}),
               bytes) &&
      str_eq((str_t){.items = rendered.items + sizeof(raw), .len = suffix.len},
             suffix);
  string_deinit(&output);
  if (!bytes_ok ||
      !format_test_case(STR_LIT("{}{}{}{}"), scalars, 4, STR_LIT("true!λ0x0")))
    return false;

  char storage[8] = {0};
  sigma_fixed_writer sink = sigma_fixed_writer_init((bytes_mut_t){
      .items = (u8 *)storage,
      .len = sizeof(storage),
  });
  sigma_writer writer = sigma_fixed_writer_as_writer(&sink);

  return !sigma_format_args(writer, STR_LIT("{}"), NULL, 0) &&
         !sigma_format_args(writer, STR_LIT("{"), basic, 1) &&
         !sigma_format_args(writer, STR_LIT("literal"), basic, 1) &&
         !sigma_format_args(writer, STR_LIT("{}"), floating, 1);
}

bool sigma_test_format_macros(void) {
  string_t output = string_init(format_test_allocator());
  sigma_string_writer sink = sigma_string_writer_init(&output);
  sigma_writer writer = sigma_string_writer_as_writer(&sink);
  i32 x = 42;
  str_t name = STR_LIT("veya");
  const void *pointer = NULL;

  bool ok = sigma_writer_fmt(writer, STR_LIT("x={} name={} "), x, name) &&
            sigma_writer_fmt(writer, STR_LIT("hello world ")) &&
            sigma_writer_fmt_lit(writer, "ptr={:p}", pointer);
  bool matches = ok && str_eq(string_view(&output),
                              STR_LIT("x=42 name=veya hello world ptr=0x0"));

  string_deinit(&output);
  return matches;
}

bool sigma_test_format_custom(void) {
  point_t point = {.x = 10, .y = 20};
  sigma_fmt_arg_t point_arg = sigma_fmt_arg_custom(&point, &point_formatter);
  sigma_fmt_arg_t invalid[] = {
      sigma_fmt_arg_custom(NULL, &point_formatter),
      sigma_fmt_arg_custom(&point, NULL),
      sigma_fmt_arg_custom(&point, &(sigma_formatter_vtable){.format = NULL}),
  };

  if (point_arg.kind != SIGMA_FMT_CUSTOM ||
      point_arg.value.custom.value != &point ||
      point_arg.value.custom.vtable != &point_formatter ||
      !format_test_case(STR_LIT("point={} debug={:?}"),
                        (sigma_fmt_arg_t[]){point_arg, point_arg}, 2,
                        STR_LIT("point=(10, 20) "
                                "debug=point_t { x: 10, y: 20 }")) ||
      !format_test_case(STR_LIT("automatic={}"),
                        (sigma_fmt_arg_t[]){sigma_fmt_arg(point)}, 1,
                        STR_LIT("automatic=(10, 20)")) ||
      format_test_case(STR_LIT("{:x}"), &point_arg, 1, STR_LIT("")))
    return false;

  char storage[1];
  sigma_fixed_writer sink = sigma_fixed_writer_init((bytes_mut_t){
      .items = (u8 *)storage,
      .len = sizeof(storage),
  });
  sigma_writer writer = sigma_fixed_writer_as_writer(&sink);

  return !sigma_format_args(writer, STR_LIT("{}"), &invalid[0], 1) &&
         !sigma_format_args(writer, STR_LIT("{}"), &invalid[1], 1) &&
         !sigma_format_args(writer, STR_LIT("{}"), &invalid[2], 1);
}

bool sigma_test_print_macro(void) {
  int fds[2];

  if (pipe(fds) != 0)
    return false;
  int saved = dup(STDOUT_FILENO);
  if (saved < 0) {
    close(fds[0]);
    close(fds[1]);
    return false;
  }
  if (dup2(fds[1], STDOUT_FILENO) != STDOUT_FILENO) {
    close(saved);
    close(fds[0]);
    close(fds[1]);
    return false;
  }

  bool printed = sigma_print("hello {} age={}\n", STR_LIT("veya"), 42);
  bool restored = dup2(saved, STDOUT_FILENO) == STDOUT_FILENO;
  close(saved);
  close(fds[1]);

  char output[64];
  ssize_t count = read(fds[0], output, sizeof(output));
  close(fds[0]);

  return printed && restored && count == 18 &&
         str_eq((str_t){.items = output, .len = (usize)count},
                STR_LIT("hello veya age=42\n"));
}

bool sigma_test_printf_wrappers(void) {
  char fixed[32];
  char too_small[4] = {'x', 'x', 'x', '\0'};

  if (!sigma_snprintf(fixed, sizeof(fixed), "value={} {}", 42, STR_LIT("ok")) ||
      str_eq(str_from_cstr(fixed), STR_LIT("value=42 ok")) == false ||
      sigma_snprintf(too_small, sizeof(too_small), "value={}", 42) ||
      too_small[0] != '\0')
    return false;

  string_t allocated = {0};
  if (!sigma_asprintf(&allocated, format_test_allocator(), "owned={} {}", 42,
                      STR_LIT("ok")) ||
      !str_eq(string_view(&allocated), STR_LIT("owned=42 ok"))) {
    string_deinit(&allocated);
    return false;
  }
  string_deinit(&allocated);

  int fds[2];
  if (pipe(fds) != 0)
    return false;
  bool printed = sigma_fprintf(fds[1], "fd={} {}", 42, STR_LIT("ok"));
  close(fds[1]);
  char output[32];
  ssize_t count = read(fds[0], output, sizeof(output));
  close(fds[0]);

  return printed && count == 8 &&
         str_eq((str_t){.items = output, .len = (usize)count},
                STR_LIT("fd=42 ok"));
}

bool sigma_test_format_parser(void) {
  sigma_fmt_field field;

  if (sigma_fmt_parse_field(STR_LIT("{}"), &field) != SIGMA_FMT_PARSE_OK ||
      field.consumed != 2 ||
      field.spec.presentation != SIGMA_FMT_PRESENT_DEFAULT ||
      sigma_fmt_parse_field(STR_LIT("{:x}"), &field) != SIGMA_FMT_PARSE_OK ||
      field.spec.presentation != SIGMA_FMT_PRESENT_HEX_LOWER ||
      sigma_fmt_parse_field(STR_LIT("{:#08x}"), &field) != SIGMA_FMT_PARSE_OK ||
      !field.spec.alternate || !field.spec.zero_pad || !field.spec.has_width ||
      field.spec.width != 8 ||
      field.spec.presentation != SIGMA_FMT_PRESENT_HEX_LOWER ||
      sigma_fmt_parse_field(STR_LIT("{:>20}"), &field) != SIGMA_FMT_PARSE_OK ||
      field.spec.align != SIGMA_FMT_ALIGN_RIGHT || field.spec.width != 20 ||
      sigma_fmt_parse_field(STR_LIT("{:*^20}"), &field) != SIGMA_FMT_PARSE_OK ||
      field.spec.fill != '*' || field.spec.align != SIGMA_FMT_ALIGN_CENTER ||
      field.spec.width != 20 ||
      sigma_fmt_parse_field(STR_LIT("{:+10d}"), &field) != SIGMA_FMT_PARSE_OK ||
      !field.spec.sign_plus || field.spec.width != 10 ||
      field.spec.presentation != SIGMA_FMT_PRESENT_DEC ||
      sigma_fmt_parse_field(STR_LIT("{:.5}"), &field) != SIGMA_FMT_PARSE_OK ||
      !field.spec.has_precision || field.spec.precision != 5 ||
      !format_parse_has_presentation(STR_LIT("{:X}"),
                                     SIGMA_FMT_PRESENT_HEX_UPPER) ||
      !format_parse_has_presentation(STR_LIT("{:b}"),
                                     SIGMA_FMT_PRESENT_BINARY) ||
      !format_parse_has_presentation(STR_LIT("{:o}"),
                                     SIGMA_FMT_PRESENT_OCTAL) ||
      !format_parse_has_presentation(STR_LIT("{:d}"), SIGMA_FMT_PRESENT_DEC) ||
      sigma_fmt_parse_field(STR_LIT("{:*>#08x}"), &field) !=
          SIGMA_FMT_PARSE_OK ||
      field.spec.fill != '*' || field.spec.align != SIGMA_FMT_ALIGN_RIGHT ||
      !field.spec.alternate || !field.spec.zero_pad || field.spec.width != 8 ||
      field.spec.presentation != SIGMA_FMT_PRESENT_HEX_LOWER)
    return false;

  return format_parse_fails(STR_LIT("{")) &&
         format_parse_fails(STR_LIT("{x}")) &&
         format_parse_fails(STR_LIT("{:")) &&
         format_parse_fails(STR_LIT("{:wat}")) &&
         format_parse_fails(STR_LIT("{:8xx}")) &&
         format_parse_fails(STR_LIT("{:.}")) &&
         format_parse_fails(STR_LIT("{:8"));
}

#if defined(SIGMA_FORMAT_FIXTURE_MAIN)
int main(void) {
  return sigma_test_format_dispatch() && sigma_test_format_runtime() &&
                 sigma_test_format_parser() && sigma_test_format_macros() &&
                 sigma_test_format_custom() && sigma_test_print_macro() &&
                 sigma_test_printf_wrappers()
             ? 0
             : 1;
}
#endif
