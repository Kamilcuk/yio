#include <yio.h>
#include <yio_test.h>
#include <string.h>

void test_align(const char *expected, const char *fmt, int val, int width) {
    char buf[256];
    int err = yio_sprintf(buf, sizeof(buf), fmt, val, width);
    YIO_TESTEXPR(err > 0, "fmt='%s' width=%d err=%d", fmt, width, err);
    YIO_TESTEXPR(strcmp(buf, expected) == 0, "Expected '%s', got '%s' (width %d)", expected, buf, width);
}

int main() {
    // Testing around 32 and 64 byte boundaries for alignment padding
    int widths[] = {31, 32, 33, 63, 64, 65};
    int num_widths = sizeof(widths)/sizeof(widths[0]);

    for (int i = 0; i < num_widths; ++i) {
        int w = widths[i];
        char expected[256];

        // Left alignment
        memset(expected, ' ', w);
        memcpy(expected, "123", 3);
        expected[w] = '\0';
        test_align(expected, "{:<{}}", 123, w);

        // Right alignment
        memset(expected, ' ', w);
        memcpy(expected + w - 3, "123", 3);
        expected[w] = '\0';
        test_align(expected, "{:>{}}", 123, w);

        // Center alignment
        memset(expected, ' ', w);
        int pad = (w - 3) / 2;
        memcpy(expected + pad, "123", 3);
        expected[w] = '\0';
        test_align(expected, "{:^{}}", 123, w);
    }

    // Testing with custom fill character to ensure chunk buffer is filled correctly
    YIO_TEST("**********123", "{:*>13}", 123);
    YIO_TEST("******************************123", "{:*>33}", 123);
    YIO_TEST("****************************************************************123", "{:*>67}", 123);

    return 0;
}
