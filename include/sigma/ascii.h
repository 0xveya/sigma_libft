#pragma once

#include <sigma/qol.h>

bool ascii_is_alnum(u8 c);
bool ascii_is_alpha(u8 c);
bool ascii_is_digit(u8 c);
bool ascii_is_lower(u8 c);
bool ascii_is_space(u8 c);
bool ascii_is_upper(u8 c);

u8 ascii_to_lower(u8 c);
u8 ascii_to_upper(u8 c);
