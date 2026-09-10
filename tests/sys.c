#include <sigma/sys.h>

static bool test_write_error(void) {
  sigma_write_result_t result = s_write(-1, "x", 1);
  return !result.ok && result.error.code == 9;
}

static bool test_map_and_unmap(void) {
  sigma_mmap_result_t mapped =
      s_mmap(NULL, 4096, SIGMA_PROT_READ | SIGMA_PROT_WRITE,
             SIGMA_MAP_PRIVATE | SIGMA_MAP_ANONYMOUS, -1, 0);
  if (!mapped.ok)
    return false;

  unsigned char *bytes = mapped.value;
  bytes[0] = 0x42;
  bytes[4095] = 0x69;
  sigma_munmap_result_t unmapped = s_munmap(mapped.value, 4096);
  return unmapped.ok;
}

int main(void) {
  return test_write_error() && test_map_and_unmap() ? 0 : 1;
}
