#include <sigma/traits.h>

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

#if defined(SIGMA_FORMAT_FIXTURE_MAIN)
int main(void) { return sigma_test_format_dispatch() ? 0 : 1; }
#endif
