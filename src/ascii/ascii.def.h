#pragma once

#define SIGMA_ASCII_RULES(RANGE, ONE, c)                                       \
  RANGE(c, 0x00, 0x1f, SIGMA_ASCII_CONTROL)                                    \
  ONE(c, 0x7f, SIGMA_ASCII_CONTROL)                                            \
  RANGE(c, '\t', '\r', SIGMA_ASCII_SPACE)                                      \
  ONE(c, ' ', SIGMA_ASCII_SPACE)                                               \
  RANGE(c, 0x20, 0x7e, SIGMA_ASCII_PRINT)                                      \
  RANGE(c, '0', '9', SIGMA_ASCII_DIGIT | SIGMA_ASCII_HEX)                      \
  RANGE(c, 'A', 'F', SIGMA_ASCII_UPPER | SIGMA_ASCII_HEX)                      \
  RANGE(c, 'G', 'Z', SIGMA_ASCII_UPPER)                                        \
  RANGE(c, 'a', 'f', SIGMA_ASCII_LOWER | SIGMA_ASCII_HEX)                      \
  RANGE(c, 'g', 'z', SIGMA_ASCII_LOWER)                                        \
  RANGE(c, '!', '/', SIGMA_ASCII_PUNCT)                                        \
  RANGE(c, ':', '@', SIGMA_ASCII_PUNCT)                                        \
  RANGE(c, '[', '`', SIGMA_ASCII_PUNCT)                                        \
  RANGE(c, '{', '~', SIGMA_ASCII_PUNCT)

#define SIGMA_ASCII_PREDICATES(X)                                              \
  X(alnum, SIGMA_ASCII_UPPER | SIGMA_ASCII_LOWER | SIGMA_ASCII_DIGIT)          \
  X(alpha, SIGMA_ASCII_UPPER | SIGMA_ASCII_LOWER)                              \
  X(cntrl, SIGMA_ASCII_CONTROL)                                                \
  X(digit, SIGMA_ASCII_DIGIT)                                                  \
  X(lower, SIGMA_ASCII_LOWER)                                                  \
  X(print, SIGMA_ASCII_PRINT)                                                  \
  X(space, SIGMA_ASCII_SPACE)                                                  \
  X(upper, SIGMA_ASCII_UPPER)                                                  \
  X(xdigit, SIGMA_ASCII_HEX)
