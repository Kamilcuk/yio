#define __STDC_WANT_IEC_60559_BFP_EXT__ 1
#define __STDC_WANT_IEC_60559_TYPES_EXT__ 1
#define __STDC_WANT_DEC_FP__ 1
#include <yio/yio_config.h>
#include <float.h>
#undef YIO_FLOAT_BACKEND
#define YIO_FLOAT_BACKEND NAIVE
// #include "yio_test_decimals.c"
int main() {
  _Static_assert(FLT_RADIX == 2, "FLT_RADIX must be 2");
  return 77;
}
