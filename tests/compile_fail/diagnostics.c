#include <sigma/libft.h>

typedef struct {
  int value;
} unregistered_t;

void sigma_diagnostic_format_type(void) {
  (void)sigma_fmt_arg((unregistered_t){0});
}

void sigma_diagnostic_move_type(void) {
  int destination = 0;
  long source = 0;

  SIGMA_MOVE(destination, source);
}

void sigma_diagnostic_character_type(void) { (void)sigma_isalpha(1.0); }

void sigma_diagnostic_deinit_type(void) {
  int value = 0;

  sigma_deinit(value);
}
