#include "conftest.h"

//// PASS_REGULAR_EXPRESSION too many arguments

int main() {
    yio_print("TEST {}", 1, 2);
    return 0;
}
