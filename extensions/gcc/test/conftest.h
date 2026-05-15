#ifndef YIO_CONFTEST_H
#define YIO_CONFTEST_H

#include <yio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define YIO_ASSERT(expr) do { \
    if (!(expr)) { \
        fprintf(stderr, "Assertion failed: %s at %s:%d\n", #expr, __FILE__, __LINE__); \
        exit(1); \
    } \
} while(0)

#define YIO_CHECK_EQ(expected, ...) do { \
    char buf[1024]; \
    yio_snprint(buf, sizeof(buf), __VA_ARGS__); \
    if (strcmp(expected, buf) != 0) { \
        fprintf(stderr, "Check failed!\nExpected: %s\nActual:   %s\nAt %s:%d\n", expected, buf, __FILE__, __LINE__); \
        exit(1); \
    } \
} while(0)

#endif
