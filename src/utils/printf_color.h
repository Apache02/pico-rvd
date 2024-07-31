#pragma once

#include <stdio.h>
#include <stdarg.h>

#define COLOR_RED         "1;31"
#define COLOR_GREEN       "1;32"
#define COLOR_YELLOW      "1;33"
#define COLOR_BLUE        "1;34"
#define COLOR_MAGENTA     "1;35"
#define COLOR_CYAN        "1;36"
#define COLOR_WHITE       "1;37"


void printf_color(const char *color, const char *fmt, ...);
