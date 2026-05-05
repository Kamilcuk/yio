#define __STDC_WANT_IEC_60559_BFP_EXT__ 1
#define __STDC_WANT_IEC_60559_TYPES_EXT__ 1
#define __STDC_WANT_DEC_FP__ 1
#include <yio/yio_config.h>
#undef YIO_FLOAT_BACKEND
#define YIO_FLOAT_BACKEND CUSTOM
// #include "yio_test_decimals.c"
int main() {
  return 77;
}
