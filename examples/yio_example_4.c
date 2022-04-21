// https://fresh2refresh.com/c-programming/c-printf-and-scanf/
#include <yio.h>
#include <time.h>
int main() {
	time_t mytime = 123;
	long long value = 456;

   	yprint(yppfmt("{:>10}"), "time", " ", yppfmt("{:<10}"), "value", "\n");
   	yprint(yppfmt("{:>10}"), mytime, " ", yppfmt("{:<10}"), value, "\n");

   	yprintf("{:>10} {:<10}\n", "time", "value");
   	yprintf("{:>10} {:<10}\n", mytime, value);
}
// PASS_REGULAR_EXPRESSION       time value[ ]*
// PASS_REGULAR_EXPRESSION        123 456[ ]*
// PASS_REGULAR_EXPRESSION       time value[ ]*
// PASS_REGULAR_EXPRESSION        123 456[ ]*
