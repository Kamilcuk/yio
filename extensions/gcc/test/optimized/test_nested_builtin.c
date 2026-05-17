#include "conftest.h"
#include <stdio.h>

//// RUN PASS_REGULAR_EXPRESSION fmt: 42

int main() {
    const int val = 42;
    
    // This is transformed by PRE_GENERICIZE first, then GIMPLE pass should fold it.
    FTEST_PRINT_F("{val}");
    
    return 0;
}
