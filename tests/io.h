#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 1. Fix _fileno and _isatty (which Linux exposes without underscores)
#include <unistd.h>
#define _fileno(stream) fileno(stream)
#define _isatty(fd)     isatty(fd)

// 2. Map gmtime_s to Linux's gmtime_r (note the inverted arguments)
static inline int gmtime_s(struct tm* buf, const time_t* time) {
    return (gmtime_r(time, buf) == NULL);
}

// 3. Inline implementations for the string conversion functions
static inline char* _custom_ultoa_impl(unsigned long value, char* buffer, int radix) {
    if (radix < 2 || radix > 36) { *buffer = '\0'; return buffer; }
    char *ptr = buffer, *low = buffer;
    do {
        unsigned long digit = value % radix;
        *ptr++ = (digit < 10) ? (char)('0' + digit) : (char)('a' + digit - 10);
        value /= radix;
    } while (value > 0);
    *ptr = '\0';
    ptr--;
    while (low < ptr) {
        char temp = *low; *low++ = *ptr; *ptr-- = temp;
    }
    return buffer;
}

static inline char* _itoa(int value, char* buffer, int radix) {
    if (radix == 10 && value < 0) {
        buffer[0] = '-';
        _custom_ultoa_impl((unsigned long)(-value), buffer + 1, radix);
        return buffer;
    }
    return _custom_ultoa_impl((unsigned long)(unsigned int)value, buffer, radix);
}

static inline char* _ltoa(long value, char* buffer, int radix) {
    if (radix == 10 && value < 0) {
        buffer[0] = '-';
        _custom_ultoa_impl((unsigned long)(-value), buffer + 1, radix);
        return buffer;
    }
    return _custom_ultoa_impl((unsigned long)value, buffer, radix);
}

static inline char* _ultoa(unsigned long value, char* buffer, int radix) {
    return _custom_ultoa_impl(value, buffer, radix);
}
