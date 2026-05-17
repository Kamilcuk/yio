#include "../conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

int main() {
    int x = 42;
    float f = 3.14;
    char *s = "hello";
    void *p = &x;

    // 3. Conversion specifiers (not supported)
    //// COMPILATION PASS_REGULAR_EXPRESSION conversion specifier .* is not supported
    print("{x!r}"); 

    // 4. Pointer and String types
    //// COMPILATION PASS_REGULAR_EXPRESSION invalid format specifier d for .*char.*use .*s.* instead
    print("{0:d}", s);
    //// COMPILATION PASS_REGULAR_EXPRESSION invalid format specifier f for .*void.*use .*p.* instead
    print("{0:f}", p);

    // 6. Mixed indexing in dynamic fields
    //// COMPILATION PASS_REGULAR_EXPRESSION cannot switch from manual field specification to automatic field numbering
    print("{0:{1}.{}f}", f, 1, 2); 
    //// COMPILATION PASS_REGULAR_EXPRESSION cannot switch from automatic field numbering to manual field specification
    print("{0:{}.{1}f}", f, 1, 2);

    // 2. Brace as fill character
    //// COMPILATION PASS_REGULAR_EXPRESSION invalid dynamic field index .*
    print("{0:{>10}}", x); 
    return 0;
}
