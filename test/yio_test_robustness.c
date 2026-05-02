/**
 * @file yio_test_robustness.c
 * @brief Comprehensive robustness tests for the Yio library.
 */
#include <yio_test.h>
#include <yio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 1. Escape sequences and literal braces */
static void test_escapes(void) {
    YIO_TEST("{", "{{");
    YIO_TEST("}", "}}");
    YIO_TEST("{}", "{{}}");
    YIO_TEST("{123}", "{{{}}}", 123);
    YIO_TEST("{123}", "{{{0}}}", 123);
    YIO_TEST(" value is {10} ", " value is {{{}}} ", 10);
}

/* 2. Recursive Callbacks */
struct Point { int x, y; };

static int yio_print_point(yio_printctx_t *t) {
    const struct Point p = yio_printctx_va_arg(t, struct Point);
    int err = yio_printctx_init(t);
    if (err) return err;
    /* Nested yio_printf call inside a callback */
    return yio_printctx_printf(t, "({:d}, {:d})", p.x, p.y);
}

static void test_recursive_callbacks(void) {
    struct Point p = {10, 20};
    YIO_TEST("(10, 20)", "{}", yio_callback(yio_print_point, p));
    YIO_TEST("Point is (10, 20)!", "Point is {}!", yio_callback(yio_print_point, p));
}

/* 3. Memory management / yio_reaprintf */
#if YIO_USE_MALLOC
static void test_memory(void) {
    char *str = NULL;
    int ret;

    /* Initial allocation */
    ret = yio_appendstream(&str, "Initial");
    YIO_TESTEXPR(ret == 7);
    YIO_TESTEXPR(strcmp(str, "Initial") == 0);

    /* Append */
    ret = yio_appendstream(&str, " and secondary");
    YIO_TESTEXPR(ret == 14);
    YIO_TESTEXPR(strcmp(str, "Initial and secondary") == 0);

    /* Large data growth */
    char large[1024];
    memset(large, 'A', sizeof(large) - 1);
    large[sizeof(large) - 1] = '\0';
    ret = yio_appendstream(&str, " plus {}", large);
    YIO_TESTEXPR(ret > 1024);
    YIO_TESTEXPR(strstr(str, "AAAAA") != NULL);

    free(str);
}
#endif

/* 4. Standardized Time Functions */
static void test_time_api(void) {
    time_t now = 1713888000; /* 2024-04-23 16:00:00 UTC */
    
    /* We expect yio_gmtime to format correctly */
    /* Note: formatting depends on locale, but %Y-%m-%d is generally stable */
    char buf[100];
    yio_snprintf(buf, sizeof(buf), "{:%Y-%m-%d}", yio_gmtime(now));
    YIO_TESTEXPR(strcmp(buf, "2024-04-23") == 0, "Got: %s", buf);

    /* Primary check */
    yio_snprintf(buf, sizeof(buf), "{:%H:%M}", yio_gmtime(now));
    YIO_TESTEXPR(strcmp(buf, "16:00") == 0, "Got: %s", buf);
}

/* 5. Edge cases for types */
static void test_type_edges(void) {
    YIO_TEST("-2147483648", "{}", -2147483647 - 1); /* INT_MIN */
    YIO_TEST("4294967295", "{}", 4294967295U);     /* UINT_MAX */
}

#include <yio/private/yio_string.h>

/* 6. SSO and Dynamic Transition */
#if YIO_USE_MALLOC
static void test_sso_transition(void) {
    YYIO_string s = {0};

    /* SSO mode */
    YIO_TESTEXPR(YYIO_string_putsn(&s, "Small", strlen("Small")) == 0);
    YIO_TESTEXPR(!YYIO_string_is_dynamic(&s));
    YIO_TESTEXPR(YYIO_string_len(&s) == 5);
    /* Need null terminator for strcmp */
    YIO_TESTEXPR(YYIO_string_putc(&s, '\0') == 0);
    YIO_TESTEXPR(strcmp(YYIO_string_data(&s), "Small") == 0);
    YYIO_string_set_used(&s, 5); /* Reset length back to 5 */

    /* Fill SSO to the limit (16 on 64-bit, 12 on 32-bit) */
    while (YYIO_string_len(&s) < YYIO_string_capacity(&s)) {
        YIO_TESTEXPR(YYIO_string_putc(&s, 'A') == 0);
    }
    YIO_TESTEXPR(!YYIO_string_is_dynamic(&s));

    /* Transition to dynamic */
    YIO_TESTEXPR(YYIO_string_putc(&s, 'B') == 0);
    YIO_TESTEXPR(YYIO_string_is_dynamic(&s));
    YIO_TESTEXPR(YYIO_string_data(&s)[YYIO_string_len(&s)-1] == 'B');

    YYIO_string_free(&s);
}
#endif

int main() {
    test_escapes();
    test_recursive_callbacks();
#if YIO_USE_MALLOC
    test_memory();
#endif
    test_time_api();
    test_type_edges();
#if YIO_USE_MALLOC
    test_sso_transition();
#endif
    return 0;
}
