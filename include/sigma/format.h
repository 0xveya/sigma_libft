#pragma once

#include <sigma/bytes.h>
#include <sigma/qol.h>
#include <sigma/rune.h>
#include <sigma/str.h>
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

/*
 * Formats an already type-erased argument array into writer.
 *
 * format and every argument are borrowed for the duration of the call.
 * Formatting never takes ownership of an argument.
 */
bool sigma_format_args(sigma_writer writer, str_t format,
                       const sigma_fmt_arg_t *args, usize arg_count);
