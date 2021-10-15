// https://fresh2refresh.com/c-programming/c-printf-and-scanf/
#include <yio.h>
int main() {
   yprintf("{:>10} {:<10}\n", "time", "value");
   yprintf("{:>10} {:<10}\n", 123, 456ull);
}
// PASS_REGULAR_EXPRESSION       time value[ ]*
// PASS_REGULAR_EXPRESSION        123 456[ ]*
