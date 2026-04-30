#include "yio.h"
#include <iostream>
#include <string>
#include <cassert>
#include <cstring>

struct MyPoint {
    int x, y;
};

static int MyPoint_print(yio_printctx_t *t) {
    MyPoint p = yio_printctx_va_arg(t, MyPoint);
    int err = yio_printctx_init(t);
    if (err) return err;
    return yio_printctx_printf(t, "({}, {})", p.x, p.y);
}

YIO_ADD_TYPE(MyPoint, MyPoint_print)
#include YIO_ADD_TYPE_INC()

int main() {
    int err;

    // Basic types
    err = yio_printf("Ints: {} {} {} {} {} {}\n", 
                     (signed char)1, (short)2, 3, 4L, 5LL, (bool)true);
    assert(err >= 0);

    err = yio_printf("UInts: {} {} {} {} {}\n", 
                     (unsigned char)1, (unsigned short)2, 3U, 4UL, 5ULL);
    assert(err >= 0);

    err = yio_printf("Floats: {:f} {:e} {:g}\n", 1.1f, 2.2, 3.3L);
    assert(err >= 0);

    // Strings and chars
    err = yio_printf("Chars: {} {} {}\n", 'A', (signed char)'B', (unsigned char)'C');
    assert(err >= 0);

    const char *str = "const char pointer";
    char str_mut[] = "mutable char array";
    err = yio_printf("Strings: {} | {}\n", str, str_mut);
    assert(err >= 0);

    // Wide chars
    err = yio_printf("Wide char: {}\n", L'X');
    assert(err >= 0);
    // Wide string: {}
    err = yio_printf("Wide string: {}\n", L"Wide string literal");
    assert(err >= 0);

    #if YIO_HAS_MALLOC
    // Positional arguments
    char *pos_str = yio_formatf("{1} {0} {1}\n", "first", "second");
    assert(pos_str != NULL);
    assert(std::string(pos_str) == "second first second\n");
    free(pos_str);

    // Custom type
    MyPoint p = {10, 20};
    char *p_str = yio_formatf("Point: {}\n", p);
    assert(p_str != NULL);
    assert(std::string(p_str) == "Point: (10, 20)\n");
    free(p_str);
    #endif

    // yio_count

    int count = 0;
    err = yio_printf("Count this: {}{}\n", "abc", yio_count(&count));
    assert(err >= 0);
    assert(count == 15);
    
#if YYIO_HAS_INT128
    __int128 i128 = 1234567890123456789;
    i128 *= 1000000;
    err = yio_printf("Int128: {}\n", i128);
    assert(err >= 0);
#endif

    std::cout << "DONE." << std::endl;
    return 0;
}
