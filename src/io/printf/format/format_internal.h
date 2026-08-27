#pragma once

#include <sigma/printf.h>
#include <sigma/writer.h>

/*
 * Result of parsing one replacement field.
 */
typedef enum {
  SIGMA_FMT_PARSE_OK,
  SIGMA_FMT_PARSE_END,
  SIGMA_FMT_PARSE_ERROR,
} sigma_fmt_parse_result;

/*
 * One parsed replacement field.
 *
 * consumed is the number of bytes consumed from the format string,
 * including the opening and closing braces.
 *
 * Example:
 *
 *   "{}"      -> consumed = 2
 *   "{:x}"    -> consumed = 4
 */
typedef struct {
  sigma_fmt_spec spec;
  usize consumed;
} sigma_fmt_field;

/*
 * Parses one replacement field beginning at input.items[0].
 *
 * input must begin with '{'.
 */
sigma_fmt_parse_result sigma_fmt_parse_field(str_t input,
                                             sigma_fmt_field *field);

/*
 * Dispatches one already type-erased argument to its formatter.
 */
bool sigma_fmt_write_arg(sigma_writer writer, sigma_fmt_arg_t arg,
                         const sigma_fmt_spec *spec);

/* Writes count copies of one ASCII format fill character. */
bool sigma_fmt_write_repeat(sigma_writer writer, u32 fill, usize count);

/*
 * Built-in value formatters.
 */
bool sigma_fmt_write_i64(sigma_writer writer, i64 value,
                         const sigma_fmt_spec *spec);

bool sigma_fmt_write_u64(sigma_writer writer, u64 value,
                         const sigma_fmt_spec *spec);

bool sigma_fmt_write_bool(sigma_writer writer, bool value,
                          const sigma_fmt_spec *spec);

bool sigma_fmt_write_char(sigma_writer writer, char value,
                          const sigma_fmt_spec *spec);

bool sigma_fmt_write_rune(sigma_writer writer, sigma_rune value,
                          const sigma_fmt_spec *spec);

bool sigma_fmt_write_ptr(sigma_writer writer, const void *value,
                         const sigma_fmt_spec *spec);

bool sigma_fmt_write_str(sigma_writer writer, str_t value,
                         const sigma_fmt_spec *spec);

bool sigma_fmt_write_bytes(sigma_writer writer, bytes_t value,
                           const sigma_fmt_spec *spec);
