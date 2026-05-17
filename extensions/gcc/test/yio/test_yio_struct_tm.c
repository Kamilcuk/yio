#include <yio.h>
#include <time.h>
#include <assert.h>
#include "conftest.h"

//// RUN
//// PASS_REGULAR_EXPRESSION "Time: 2023-10-27 12:34:56"

int main() {
    struct tm t;
    t.tm_year = 2023 - 1900;
    t.tm_mon = 9;
    t.tm_mday = 27;
    t.tm_hour = 12;
    t.tm_min = 34;
    t.tm_sec = 56;
    t.tm_isdst = -1;

    // Use yio_print_f which internally uses f-string logic.
    // struct tm is NOT optimizable, so it should be handled by the library's runtime.
    int err = yio_print_f("Time: {t:%Y-%m-%d %H:%M:%S}\n");
    assert(err > 0);
    return 0;
}
