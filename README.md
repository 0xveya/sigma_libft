libft remake for post cc 42 stuff where i will make a package manger for to ingrerate nicely with sigma_malloc and sigma_rt so that simga_npm will be sigma

this libft is crap so far i will step by stel improve it

function documentation strings are shamelessly ai generated for the better of
everyone because thats how we get them at all and without 7 trillion typos :sob:


todo:
- debug moved-from poisoning belongs in sigma_rt
- unicode special casing needs a string-producing api, rune to rune can only do simple casing
- normalization and grapheme iteration once the utf-8 iterator exists
- add syscall wrappers
- remove hidden alcations
- fix all like sigma_annotations
- measure before doing cursed optimizations

unicode.bin is generated from the pinned Unicode 17.0.0 UnicodeData.txt,
DerivedCoreProperties.txt and PropList.txt files. build the generator and pass
those three files plus the output path explicitly:

```sh
zig cc -std=c23 -Wall -Wextra -Wpedantic -pedantic-errors \
  tools/unicode_gen.c -o /tmp/sigma-unicode-gen
/tmp/sigma-unicode-gen \
  /path/to/UnicodeData.txt \
  /path/to/DerivedCoreProperties.txt \
  /path/to/PropList.txt \
  src/unicode/unicode.bin
```

the generated database derives from the Unicode Character Database and is
covered by the Unicode terms of use: https://www.unicode.org/terms_of_use.html
