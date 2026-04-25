#include <yio.h>
#include <yio_test.h>
#include <string.h>
#include <stdlib.h>

int main() {
    // Test dynamic growth in yio_formatf
    // Initial capacity is 32. We will append many chunks to trigger multiple reallocs.
    char *str = yio_formatf("{}{}{}{}{}{}{}{}{}{}", 
                           "0123456789", "0123456789", "0123456789", "0123456789", "0123456789",
                           "0123456789", "0123456789", "0123456789", "0123456789", "0123456789");
    
    YIO_TESTEXPR(str != NULL);
    YIO_TESTEXPR(strlen(str) == 100);
    for (int i = 0; i < 100; ++i) {
        YIO_TESTEXPR(str[i] == '0' + (i % 10));
    }
    free(str);

    // Test a very large single string to ensure we jump past YYIO_INIT_CAPACITY correctly
    char *large_src = malloc(1025);
    memset(large_src, 'A', 1024);
    large_src[1024] = '\0';
    
    char *large_res = yio_formatf("{}", large_src);
    YIO_TESTEXPR(large_res != NULL);
    YIO_TESTEXPR(strlen(large_res) == 1024);
    YIO_TESTEXPR(large_res[0] == 'A');
    YIO_TESTEXPR(large_res[1023] == 'A');
    
    free(large_res);
    free(large_src);

    return 0;
}
