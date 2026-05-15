#include <yio.h>
#include <yio_test.h>
#include <locale.h>

#ifndef YIO_HAS_UNISTRING
#error YIO_HAS_UNISTRING
#endif

int main() {
        // we need UTF-8 locale to work here
        char *ret = setlocale(LC_ALL, "en_US.UTF-8");
        if (!ret) {
                ret = setlocale(LC_ALL, "C.UTF-8");
                if (!ret) {
                        //// SKIP_RETURN_CODE 100
                        return 100;
                }
        }

#if YIO_HAS_UNISTRING && YIO_ENABLE_MALLOC
        // This needs unistring support from u8_width
        char *str = NULL;
        int err = yio_asprint(&str, "{:5s}", "Ω");
        YIO_TESTEXPR(err > 0, "err=%d", err);
        YIO_TESTEXPR(strcmp(str, "Ω    ") == 0, "'%s'", str);
        YIO_TESTEXPR(strlen(str) == strlen("Ω    "));
        free(str);
#endif
}
