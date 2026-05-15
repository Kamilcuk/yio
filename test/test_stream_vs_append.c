/**
 * @file test_stream_vs_append.c
 * @brief Regression test to ensure clear distinction between stream and printf/append APIs.
 */
#include <yio_test.h>
#include <yio.h>
#include <string.h>
#include <stdlib.h>

int main() {
#if YIO_ENABLE_MALLOC
    char *str = NULL;
    int ret;

    /* 1. yio_asstream vs yio_asprint */
    
    // asstream should NOT format (it treats placeholders as literal strings)
    ret = yio_asstream(&str, "literal {}", 123);
    YIO_TESTEXPR(ret == 13);
    YIO_TESTEXPR(strcmp(str, "literal {}123") == 0, "Got: %s", str);
    
    free(str); str = NULL;
    ret = yio_asstream(&str, "val=", 123);
    YIO_TESTEXPR(strcmp(str, "val=123") == 0, "Got: %s", str);

    // asprintf MUST format
    free(str); str = NULL;
    ret = yio_asprint(&str, "val={}", 123);
    YIO_TESTEXPR(strcmp(str, "val=123") == 0, "Got: %s", str);
    
    /* 2. yio_appendstream vs yio_append */
    
    free(str); str = NULL;
    yio_asstream(&str, "base");
    
    // appendstream appends raw data
    yio_appendstream(&str, " + ", 456);
    YIO_TESTEXPR(strcmp(str, "base + 456") == 0, "Got: %s", str);
    
    // append appends formatted data
    yio_append(&str, " + {}", 789);
    YIO_TESTEXPR(strcmp(str, "base + 456 + 789") == 0, "Got: %s", str);
    
    free(str);
#endif

    return 0;
}
