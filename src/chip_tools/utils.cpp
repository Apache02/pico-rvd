#include "utils.h"
#include <stdarg.h>
#include <stdio.h>

//------------------------------------------------------------------------------

void printf_color(const char *color, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("\u001b[%sm", color);
    vprintf(fmt, args);
    printf("\u001b[0m");
}

//------------------------------------------------------------------------------

char to_hex(int x) {
    if (x >= 0 && x <= 9) return '0' + x;
    if (x >= 10 && x <= 15) return 'A' - 10 + x;
    return '?';
}

int from_hex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + c - 'a';
    if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    return -1;
}

bool from_hex(char c, int &out) {
    if (c >= '0' && c <= '9') {
        out = c - '0';
        return true;
    }
    if (c >= 'a' && c <= 'f') {
        out = 10 + c - 'a';
        return true;
    }
    if (c >= 'A' && c <= 'F') {
        out = 10 + c - 'A';
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------

int cmp(const char *prefix, const char *text) {
    while (1) {
        // Note we return when we run out of matching prefix, not when both
        // strings match.
        if (*prefix == 0) return 0;
        if (*prefix > *text) return -1;
        if (*prefix < *text) return 1;
        prefix++;
        text++;
    }
}
