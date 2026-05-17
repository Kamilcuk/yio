#include "conftest.h"
#include <stdio.h>

static int h_double(ftest_ctx *ctx) {
  if (ctx) {
    double val = va_arg(*ctx, double);
    printf("h_double: %f\n", val);
  }
  return 0;
}

#define HANDLERS (double *)0, h_double

int main() {
    double f = 3.14159;
    int width = 10;
    int precision = 2;

    //// RUN PASS_REGULAR_EXPRESSION fmt: {:0{}.{}f}.*h_double: 3.141590.*h_int: 10.*h_int: 2
    ftest_call(FTEST_F("{f:0{width}.{precision}f}"), f, width, precision);

    return 0;
}
