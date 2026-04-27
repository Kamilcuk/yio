#include <yio.h>
#include <yio_test.h>
#include <string.h>

int main() {
    char buf[21]; // Size 21
    int err;

    // 1. Exact fit: 20 chars + null
    // "12345678901234567890" = 20 chars
    err = yio_snprintf(buf, 21, "{}{}", "1234567890", "1234567890");
    YIO_TESTEXPR(err == 20, "err=%d", err);
    YIO_TESTEXPR(strlen(buf) == 20);
    YIO_TESTEXPR(strcmp(buf, "12345678901234567890") == 0);

    // 2. Over by 1: 21 chars attempted, 20 allowed
    // First part 10 chars, second part 11 chars
    err = yio_snprintf(buf, 21, "{}{}", "1234567890", "12345678901");
    YIO_TESTEXPR(err == YIO_ERROR_ENOBUFS, "Expected %d, got %d", YIO_ERROR_ENOBUFS, err);
    YIO_TESTEXPR(strlen(buf) == 20);
    YIO_TESTEXPR(strcmp(buf, "12345678901234567890") == 0);

    // 3. Over in the middle of first chunk
    // Buffer size 5. Chunks: "ABC", "DEFG"
    // "ABC" (3) fits. "DE" (2) would fit but needs null.
    // So "ABCD" + null.
    char small[5];
    err = yio_snprintf(small, 5, "{}{}", "ABC", "DEFG");
    YIO_TESTEXPR(err == YIO_ERROR_ENOBUFS, "Expected %d, got %d", YIO_ERROR_ENOBUFS, err);
    YIO_TESTEXPR(strlen(small) == 4);
    YIO_TESTEXPR(strcmp(small, "ABCD") == 0);

    // 4. Multiple interleaved writes
    // buf size 10. Chunks: "123", "456", "789", "012"
    char buf10[10];
    err = yio_snprintf(buf10, 10, "{}{}{}{}", "123", "456", "789", "012");
    YIO_TESTEXPR(err == YIO_ERROR_ENOBUFS, "Expected %d, got %d", YIO_ERROR_ENOBUFS, err);
    YIO_TESTEXPR(strlen(buf10) == 9);
    YIO_TESTEXPR(strcmp(buf10, "123456789") == 0);

    return 0;
}
