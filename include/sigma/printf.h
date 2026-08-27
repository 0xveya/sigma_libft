#pragma once

#include <sigma/bytes.h>
#include <sigma/diagnostic.h>
#include <sigma/meta.h>
#include <sigma/qol.h>
#include <sigma/rune.h>
#include <sigma/str.h>
#include <sigma/type_registry.h>
#include <sigma/writer.h>

#include <stdbool.h>

/*
 * Presentation requested by a format field.
 *
 * Presentation describes HOW a value is rendered, not its C type. The value
 * type is carried separately by sigma_fmt_arg.
 *
 * Examples:
 *
 *   {}      SIGMA_FMT_PRESENT_DEFAULT
 *   {:d}    SIGMA_FMT_PRESENT_DEC
 *   {:x}    SIGMA_FMT_PRESENT_HEX_LOWER
 *   {:X}    SIGMA_FMT_PRESENT_HEX_UPPER
 *   {:b}    SIGMA_FMT_PRESENT_BINARY
 *   {:o}    SIGMA_FMT_PRESENT_OCTAL
 *   {:p}    SIGMA_FMT_PRESENT_POINTER
 *   {:?}    SIGMA_FMT_PRESENT_DEBUG
 */
typedef enum {
  SIGMA_FMT_PRESENT_DEFAULT,
  SIGMA_FMT_PRESENT_DEC,
  SIGMA_FMT_PRESENT_HEX_LOWER,
  SIGMA_FMT_PRESENT_HEX_UPPER,
  SIGMA_FMT_PRESENT_BINARY,
  SIGMA_FMT_PRESENT_OCTAL,
  SIGMA_FMT_PRESENT_POINTER,
  SIGMA_FMT_PRESENT_DEBUG,
} sigma_fmt_presentation;

/*
 * Alignment requested by a format field.
 *
 * SIGMA_FMT_ALIGN_DEFAULT allows each formatter to choose its natural
 * alignment. Numbers will normally align right while strings align left.
 */
typedef enum {
  SIGMA_FMT_ALIGN_DEFAULT,
  SIGMA_FMT_ALIGN_LEFT,
  SIGMA_FMT_ALIGN_RIGHT,
  SIGMA_FMT_ALIGN_CENTER,
} sigma_fmt_align;

/*
 * Parsed presentation options for one replacement field.
 *
 * This structure contains only presentation information. It does not describe
 * the type of the argument being formatted.
 */
typedef struct {
  usize width;
  usize precision;

  u32 fill;

  sigma_fmt_align align;
  sigma_fmt_presentation presentation;

  bool has_width;
  bool has_precision;

  bool alternate;
  bool zero_pad;
  bool sign_plus;
  bool sign_space;
} sigma_fmt_spec;

/*
 * Custom formatter interface.
 *
 * value is borrowed for the duration of the call. A formatter must not move,
 * mutate, deinitialize, free, or otherwise take ownership of it.
 */
typedef struct sigma_formatter_vtable {
  bool (*format)(sigma_writer writer, const void *value,
                 const sigma_fmt_spec *spec);
} sigma_formatter_vtable;

/* Type-erased borrowed custom formatting value and its formatter. */
typedef struct {
  const void *value;
  const sigma_formatter_vtable *vtable;
} sigma_fmt_custom;

/*
 * Runtime category of a type-erased formatting argument.
 *
 * Integer values are normalized to i64/u64 and floating-point values to f64.
 * Views remain borrowed views and are never copied or consumed.
 */
typedef enum {
  SIGMA_FMT_BOOL,

  SIGMA_FMT_I64,
  SIGMA_FMT_U64,
  SIGMA_FMT_F64,

  SIGMA_FMT_CHAR,
  SIGMA_FMT_RUNE,

  SIGMA_FMT_STR,
  SIGMA_FMT_BYTES,

  SIGMA_FMT_PTR,
  SIGMA_FMT_CUSTOM,
} sigma_fmt_kind;

/*
 * Type-erased borrowed formatting argument.
 *
 * sigma_fmt_arg never owns resources. Creating one from a view copies only the
 * view itself. The referenced storage must remain alive for the duration of
 * formatting.
 */
typedef struct {
  sigma_fmt_kind kind;

  union {
    bool boolean;

    i64 signed_integer;
    u64 unsigned_integer;
    f64 floating;

    char character;
    sigma_rune rune;

    str_t str;
    bytes_t bytes;

    const void *pointer;
    sigma_fmt_custom custom;
  } value;
} sigma_fmt_arg_t;

/*
 * Creates formatting arguments from concrete values.
 *
 * These functions do not allocate and do not take ownership.
 */
static inline sigma_fmt_arg_t sigma_fmt_arg_bool(bool value) {
  return (sigma_fmt_arg_t){
      .kind = SIGMA_FMT_BOOL,
      .value.boolean = value,
  };
}

static inline sigma_fmt_arg_t sigma_fmt_arg_i64(i64 value) {
  return (sigma_fmt_arg_t){
      .kind = SIGMA_FMT_I64,
      .value.signed_integer = value,
  };
}

static inline sigma_fmt_arg_t sigma_fmt_arg_u64(u64 value) {
  return (sigma_fmt_arg_t){
      .kind = SIGMA_FMT_U64,
      .value.unsigned_integer = value,
  };
}

static inline sigma_fmt_arg_t sigma_fmt_arg_f32(f32 value) {
  return (sigma_fmt_arg_t){
      .kind = SIGMA_FMT_F64,
      .value.floating = (f64)value,
  };
}

static inline sigma_fmt_arg_t sigma_fmt_arg_f64(f64 value) {
  return (sigma_fmt_arg_t){
      .kind = SIGMA_FMT_F64,
      .value.floating = value,
  };
}

static inline sigma_fmt_arg_t sigma_fmt_arg_char(char value) {
  return (sigma_fmt_arg_t){
      .kind = SIGMA_FMT_CHAR,
      .value.character = value,
  };
}

static inline sigma_fmt_arg_t sigma_fmt_arg_rune(sigma_rune value) {
  return (sigma_fmt_arg_t){
      .kind = SIGMA_FMT_RUNE,
      .value.rune = value,
  };
}

static inline sigma_fmt_arg_t sigma_fmt_arg_str(str_t value) {
  return (sigma_fmt_arg_t){
      .kind = SIGMA_FMT_STR,
      .value.str = value,
  };
}

static inline sigma_fmt_arg_t sigma_fmt_arg_bytes(bytes_t value) {
  return (sigma_fmt_arg_t){
      .kind = SIGMA_FMT_BYTES,
      .value.bytes = value,
  };
}

static inline sigma_fmt_arg_t sigma_fmt_arg_ptr(const void *value) {
  return (sigma_fmt_arg_t){
      .kind = SIGMA_FMT_PTR,
      .value.pointer = value,
  };
}

/* Creates a borrowed argument dispatched through a custom formatter. */
static inline sigma_fmt_arg_t
sigma_fmt_arg_custom(const void *value, const sigma_formatter_vtable *vtable) {
  return (sigma_fmt_arg_t){
      .kind = SIGMA_FMT_CUSTOM,
      .value.custom =
          (sigma_fmt_custom){
              .value = value,
              .vtable = vtable,
          },
  };
}

/*
 * Formats an already type-erased argument array into writer.
 *
 * format and every argument are borrowed for the duration of the call.
 * Formatting never takes ownership of an argument.
 */
bool sigma_format_args(sigma_writer writer, str_t format,
                       const sigma_fmt_arg_t *args, usize arg_count);

/* Pointer-taking adapters keep copied values alive through the format call. */
#define SIGMA_FORMAT_REF_DEFINE(tag, type, constructor)                        \
  static inline sigma_fmt_arg_t SIGMA_CAT(sigma_fmt_arg_ref_,                  \
                                          tag)(type const *value) {            \
    return constructor(*value);                                                \
  }
SIGMA_FORMAT_TYPES(SIGMA_FORMAT_REF_DEFINE)

/* Registered custom values use the same storage but remain borrowed. */
#define SIGMA_CUSTOM_FORMAT_REF_DEFINE(tag, type, formatter)                   \
  static inline sigma_fmt_arg_t SIGMA_CAT(sigma_fmt_arg_ref_,                  \
                                          tag)(type const *value) {            \
    return sigma_fmt_arg_custom(value, &(formatter));                          \
  }
SIGMA_CUSTOM_FORMAT_TYPES(SIGMA_CUSTOM_FORMAT_REF_DEFINE)

#define SIGMA_FORMAT_REF_ASSOC(tag, type, constructor)                         \
  type * : SIGMA_CAT(sigma_fmt_arg_ref_, tag),
#define SIGMA_CUSTOM_FORMAT_REF_ASSOC(tag, type, formatter)                    \
  type * : SIGMA_CAT(sigma_fmt_arg_ref_, tag),
#define SIGMA_FORMAT_REF_SUPPORTED_ASSOC(tag, type, constructor) type * : true,
#define SIGMA_CUSTOM_FORMAT_REF_SUPPORTED_ASSOC(tag, type, formatter)          \
  type * : true,
#define SIGMA_FMT_REF_TYPE_SUPPORTED(value_ref)                                \
  _Generic((value_ref),                                                        \
      SIGMA_FORMAT_TYPES(SIGMA_FORMAT_REF_SUPPORTED_ASSOC)                     \
          SIGMA_CUSTOM_FORMAT_TYPES(                                           \
               SIGMA_CUSTOM_FORMAT_REF_SUPPORTED_ASSOC) default: false)
#define SIGMA_FMT_ARG_REF(value_ref, source)                                      \
  SIGMA_REQUIRE_EXPR(                                                             \
      SIGMA_FMT_REF_TYPE_SUPPORTED(value_ref), "SIGMA_E_FORMAT_TYPE",             \
      "unsupported formatting argument type", source,                             \
      "register the type in SIGMA_FORMAT_TYPES or "                               \
      "SIGMA_CUSTOM_FORMAT_TYPES",                                                \
      _Generic((value_ref),                                                       \
          SIGMA_FORMAT_TYPES(SIGMA_FORMAT_REF_ASSOC)                              \
              SIGMA_CUSTOM_FORMAT_TYPES(SIGMA_CUSTOM_FORMAT_REF_ASSOC) default: ( \
                   sigma_fmt_arg_t (*)(typeof(value_ref)))0)(value_ref))

#undef sigma_fmt_arg
/* Copies builtins and block-borrows registered custom values exactly once. */
#define SIGMA_FMT_ARG_STORAGE(value)                                           \
  (&((union { typeof_unqual(value) item; }){.item = (value)}.item))
#define sigma_fmt_arg(value)                                                   \
  SIGMA_FMT_ARG_REF(SIGMA_FMT_ARG_STORAGE(value), value)

#undef SIGMA_FMT_TYPE_SUPPORTED
#define SIGMA_FMT_TYPE_SUPPORTED(value)                                        \
  SIGMA_FMT_REF_TYPE_SUPPORTED(SIGMA_FMT_ARG_STORAGE(value))

#define SIGMA_FMT_MAP_ARG(value) sigma_fmt_arg(value)

#define SIGMA_FMT_ARGS(...)                                                    \
  ((sigma_fmt_arg_t[]){                                                        \
       {0} __VA_OPT__(, SIGMA_PP_MAP(SIGMA_FMT_MAP_ARG, __VA_ARGS__))} +       \
   1)

#define sigma_writer_fmt(writer, format, ...)                                  \
  sigma_format_args((writer), (format), SIGMA_FMT_ARGS(__VA_ARGS__),           \
                    SIGMA_PP_NARGS(__VA_ARGS__))

#define sigma_writer_fmt_lit(writer, format, ...)                              \
  sigma_writer_fmt((writer), STR_LIT(format) __VA_OPT__(, __VA_ARGS__))

/* Formats borrowed arguments to an fd and reports write/format failure. */
bool sigma_fprint_args(int fd, str_t format, const sigma_fmt_arg_t *args,
                       usize arg_count);

/*
 * Formats into buffer and always reserves one byte for NUL.
 *
 * Returns false for a null/empty buffer, invalid formatting, or truncation.
 */
bool sigma_snprint_args(char *buffer, usize capacity, str_t format,
                        const sigma_fmt_arg_t *args, usize arg_count);

/*
 * Allocates a formatted owning string without changing out on failure.
 *
 * out must not already own storage. The caller owns the result on success.
 */
bool sigma_asprint_args(string_t *out, allocator_t allocator, str_t format,
                        const sigma_fmt_arg_t *args, usize arg_count);

/* Formats borrowed arguments to stdout. */
bool sigma_print_args(str_t format, const sigma_fmt_arg_t *args,
                      usize arg_count);

#define sigma_fprintf(fd, format, ...)                                         \
  sigma_fprint_args((fd), STR_LIT(format), SIGMA_FMT_ARGS(__VA_ARGS__),        \
                    SIGMA_PP_NARGS(__VA_ARGS__))
#define sigma_snprintf(buffer, capacity, format, ...)                          \
  sigma_snprint_args((buffer), (capacity), STR_LIT(format),                    \
                     SIGMA_FMT_ARGS(__VA_ARGS__), SIGMA_PP_NARGS(__VA_ARGS__))
#define sigma_asprintf(out, allocator, format, ...)                            \
  sigma_asprint_args((out), (allocator), STR_LIT(format),                      \
                     SIGMA_FMT_ARGS(__VA_ARGS__), SIGMA_PP_NARGS(__VA_ARGS__))
#define sigma_printf(format, ...)                                              \
  sigma_print_args(STR_LIT(format), SIGMA_FMT_ARGS(__VA_ARGS__),               \
                   SIGMA_PP_NARGS(__VA_ARGS__))
#define sigma_print(format, ...) sigma_printf(format __VA_OPT__(, __VA_ARGS__))
