#include "printf_color.h"

void printf_color(const char* color, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("\u001b[%sm", color);
    vprintf(fmt, args);
    printf("\u001b[0m");
}

