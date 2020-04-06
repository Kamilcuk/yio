// https://fresh2refresh.com/c-programming/c-printf-and-scanf/
#include <yio.h>
#include <time.h>
int main() {
   yprint(yppfmt("{:>10}"), "time", " ", yppfmt("{:<10}"), "value", "\n");
   yprint(yppfmt("{:>10}"), (time_t)123, " ", yppfmt("{:<10}"), 456, "\n");
}
// PASS_REGULAR_EXPRESSION       time value
// PASS_REGULAR_EXPRESSION        123 456
