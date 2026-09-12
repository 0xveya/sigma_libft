#include <sigma/str.h>

/** Performs the str find byte operation. */
usize str_find_byte(str_t s, u8 byte) { return bytes_find(str_bytes(s), byte); }

/** Performs the str rfind byte operation. */
usize str_rfind_byte(str_t s, u8 byte) {
  return bytes_rfind(str_bytes(s), byte);
}
