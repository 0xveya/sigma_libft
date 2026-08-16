#pragma once

#include <sigma/libft.h>

#define FT_PRINTF_BUFFER_SIZE 4096

typedef enum ft_conversion {
  conv_none,
  conv_char,
  conv_str,
  conv_ptr,
  conv_int,
  conv_uint,
  conv_hex_low,
  conv_hex_up,
  conv_percent,
} ft_conversion;

typedef enum ft_format_flag {
  FMT_MINUS = 1U << 0,
  FMT_ZERO = 1U << 1,
  FMT_HASH = 1U << 2,
  FMT_PLUS = 1U << 3,
  FMT_SPACE = 1U << 4,
} ft_format_flag;

typedef struct ft_format {
  u32 flags;
  i32 width;
  i32 precision;
  ft_conversion type;
} ft_format;

typedef struct ft_integer_format {
  u64 magnitude;
  i32 sign;
  i32 digits_len;
  i32 zeroes;
  i32 pad;
} ft_integer_format;

typedef struct ft_writer {
  char buf[FT_PRINTF_BUFFER_SIZE];
  usize len;
  i32 total;
  i32 error;
} ft_writer;

#define t_conv ft_conversion
#define t_format ft_format
#define t_intfmt ft_integer_format
#define t_writer ft_writer

t_conv ft_char_to_conv(char c);
u32 ft_char_to_flag(char c);
i32 ft_format_has(const t_format *format, u32 flag);
void ft_format_init(t_format *format);
i32 ft_dispatch_print(t_writer *writer, t_format *format, va_list args);
i32 ft_parse_format(const char *text, i32 index, t_format *format);
i32 ft_writer_char(t_writer *writer, char c);
i32 ft_writer_flush(t_writer *writer);
i32 ft_writer_repeat(t_writer *writer, char c, usize count);
i32 ft_writer_write(t_writer *writer, const char *text, usize size);
i32 ft_print_char_fmt(t_writer *writer, i32 c, t_format *format);
i32 ft_print_hex_low_fmt(t_writer *writer, unsigned int value,
                         t_format *format);
i32 ft_print_hex_up_fmt(t_writer *writer, unsigned int value, t_format *format);
i32 ft_print_int_fmt(t_writer *writer, i32 value, t_format *format);
i32 ft_print_percent_fmt(t_writer *writer, t_format *format);
i32 ft_print_ptr_fmt(t_writer *writer, void *ptr, t_format *format);
i32 ft_print_str_fmt(t_writer *writer, const char *text, t_format *format);
i32 ft_print_uint_fmt(t_writer *writer, unsigned int value, t_format *format);
char *ft_u64_base(char *end, u64 value, u32 base, const char *digits);
char *ft_u64_dec(char *end, u64 value);
char *ft_u64_pow2(char *end, u64 value, u32 shift, const char *digits);
