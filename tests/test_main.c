#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool sigma_test_vec_codegen(void);
bool sigma_test_string_ownership(void);
bool sigma_test_format_dispatch(void);
bool sigma_test_format_runtime(void);
bool sigma_test_format_parser(void);
bool sigma_test_meta(void);
bool sigma_test_format_macros(void);
bool sigma_test_format_custom(void);
bool sigma_test_print_macro(void);
bool sigma_test_printf_wrappers(void);
bool sigma_test_core(void);
bool sigma_test_conversion_and_memory(void);
bool sigma_test_lists(void);
bool sigma_test_strings_and_bytes(void);
bool sigma_test_characters(void);
bool sigma_test_collections_and_reader(void);

typedef struct {
  const char *name;
  bool (*run)(void);
} test_case_t;

int main(int argc, char **argv) {
  const test_case_t tests[] = {
      {"vector codegen preserves ownership contracts", sigma_test_vec_codegen},
      {"owned strings and string vectors preserve ownership contracts",
       sigma_test_string_ownership},
      {"format arguments normalize values and borrow views",
       sigma_test_format_dispatch},
      {"runtime formatting writes default integers and borrowed strings",
       sigma_test_format_runtime},
      {"Sigma format fields parse sequential presentation options",
       sigma_test_format_parser},
      {"preprocessor argument counting and mapping cover zero through 64",
       sigma_test_meta},
      {"format convenience macros map values and accept no arguments",
       sigma_test_format_macros},
      {"custom formatter vtables borrow and dispatch values",
       sigma_test_format_custom},
      {"sigma_print formats literals to stdout", sigma_test_print_macro},
      {"printf wrappers target buffers, fds, and owning strings",
       sigma_test_printf_wrappers},
      {"parse i32 distinguishes valid, invalid, and overflow input",
       sigma_test_conversion_and_memory},
      {"memory primitives handle exact byte ranges",
       sigma_test_conversion_and_memory},
      {"SIMD memory paths preserve guards and tails",
       sigma_test_conversion_and_memory},
      {"string map grows and retrieves values",
       sigma_test_collections_and_reader},
      {"line reader preserves buffered lines and reports eof",
       sigma_test_collections_and_reader},
      {"ASCII table classifies and converts every byte", sigma_test_characters},
      {"Unicode database classifies and maps representative runes",
       sigma_test_characters},
      {"ft_atoi", sigma_test_conversion_and_memory},
      {"ft_itoa", sigma_test_conversion_and_memory},
      {"ft_bzero", sigma_test_conversion_and_memory},
      {"ft_calloc", sigma_test_conversion_and_memory},
      {"ft_memchr", sigma_test_conversion_and_memory},
      {"ft_memcmp", sigma_test_conversion_and_memory},
      {"ft_memcpy", sigma_test_conversion_and_memory},
      {"ft_memmove", sigma_test_conversion_and_memory},
      {"ft_memset", sigma_test_conversion_and_memory},
      {"ft_lstnew", sigma_test_lists},
      {"ft_lstadd_front", sigma_test_lists},
      {"ft_lstadd_back", sigma_test_lists},
      {"ft_lstlast", sigma_test_lists},
      {"ft_lstsize", sigma_test_lists},
      {"ft_lstiter", sigma_test_lists},
      {"ft_lstdelone", sigma_test_lists},
      {"ft_lstclear", sigma_test_lists},
      {"ft_lstmap", sigma_test_lists},
      {"str converts from and to C strings", sigma_test_strings_and_bytes},
      {"str slices and compares bounded contents",
       sigma_test_strings_and_bytes},
      {"str finds bytes and trims ASCII whitespace",
       sigma_test_strings_and_bytes},
      {"str scalar split preserves empty fields", sigma_test_strings_and_bytes},
      {"bytes slices immutable and mutable views",
       sigma_test_strings_and_bytes},
      {"bytes compares bounded contents", sigma_test_strings_and_bytes},
      {"bytes finds values within bounds", sigma_test_strings_and_bytes},
      {"modern memory primitives stay within bounds",
       sigma_test_conversion_and_memory},
      {"SIMD find and compare handle vector boundaries and tails",
       sigma_test_conversion_and_memory},
      {"sigma_str_hash", sigma_test_collections_and_reader},
      {"sigma_str_map_init and deinit", sigma_test_collections_and_reader},
      {"sigma_str_map_put", sigma_test_collections_and_reader},
      {"sigma_str_map_get", sigma_test_collections_and_reader},
      {"sigma_str_map_has", sigma_test_collections_and_reader},
      {"sigma_line_reader_init and deinit", sigma_test_collections_and_reader},
      {"sigma_line_reader_next", sigma_test_collections_and_reader},
      {"line reader scans long lines and clears its arena",
       sigma_test_collections_and_reader},
  };

  if (argc == 2) {
    for (size_t index = 0; index < sizeof(tests) / sizeof(*tests); ++index) {
      if (strcmp(argv[1], tests[index].name) == 0)
        return tests[index].run() ? 0 : 1;
    }
    fprintf(stderr, "unknown test: %s\n", argv[1]);
    return 2;
  }

  for (size_t index = 0; index < sizeof(tests) / sizeof(*tests); ++index) {
    if (!tests[index].run()) {
      fprintf(stderr, "failed: %s\n", tests[index].name);
      return 1;
    }
  }
  return 0;
}
