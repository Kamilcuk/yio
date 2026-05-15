#include "conftest.h"
#include <stdbool.h>

//// PASS_REGULAR_EXPRESSION unclosed replacement field
void test_unclosed() {
    yio_print("TEST {");
}

//// PASS_REGULAR_EXPRESSION unmatched }
void test_unmatched() {
    yio_print("TEST }");
}

//// PASS_REGULAR_EXPRESSION invalid format specification
void test_invalid_spec() {
    yio_print("{:???}", 1);
}

//// PASS_REGULAR_EXPRESSION invalid format specifier 'f' for bool
void test_bool_f() {
    yio_print("{:f}", true);
}

//// PASS_REGULAR_EXPRESSION dynamic width argument 1 must be an integer
void test_dynamic_width_type() {
    yio_print("{:{}d}", 42, "string");
}

//// PASS_REGULAR_EXPRESSION dynamic precision argument 1 must be an integer
void test_dynamic_precision_type() {
    yio_print("{:.{}f}", 1.2, 1.2);
}

int main() {
    return 0;
}
