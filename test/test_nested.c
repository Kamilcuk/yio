#include <yio.h>
#include <yio_test.h>
#include <stdbool.h>

int main() {
    // Nested width and precision
    YIO_TEST("       123", "{:{}}", 123, 10);
    YIO_TEST("123.46", "{:.{}f}", 123.456, 2);
    YIO_TEST("  123.46", "{:{}.{}f}", 123.456, 8, 2);

    // Positional nested width and precision
    YIO_TEST("       123", "{0:{1}}", 123, 10);
    YIO_TEST("123.46", "{0:.{1}f}", 123.456, 2);
    YIO_TEST("  123.46", "{0:{1}.{2}f}", 123.456, 8, 2);

    // Mixed indexing
    YIO_TEST("  123.46", "{0:{}.{2}f}", 123.456, 8, 2);

    // Testing types for width/precision
    YIO_TEST("       123", "{:{}}", 123, (char)10);
    YIO_TEST("       123", "{:{}}", 123, (short)10);
    YIO_TEST("123", "{:{}}", 123, true); // true -> 1

    // Invalid types for width/precision
    {
        char buf[100];
        int err = yio_snprintf(buf, sizeof(buf), "{:{}}", 123, "abc");
        YIO_TESTEXPR(err < 0);
        YIO_TESTEXPR(err == YIO_ERROR_DYNAMIC_NOT_NUMBER, "err=%d expected=%d", err, YIO_ERROR_DYNAMIC_NOT_NUMBER);
    }

    // Negative width/precision
    {
        char buf[100];
        int err = yio_snprintf(buf, sizeof(buf), "{:{}}", 123, -5);
        YIO_TESTEXPR(err < 0);
        YIO_TESTEXPR(err == YIO_ERROR_DYNAMIC_NEGATIVE, "err=%d expected=%d", err, YIO_ERROR_DYNAMIC_NEGATIVE);
    }

    return 0;
}
