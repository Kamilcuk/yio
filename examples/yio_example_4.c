// https://fresh2refresh.com/c-programming/c-printf-and-scanf/
#include <yio.h>
#include <time.h>
int main() {
	time_t mytime = 123;
	long long value = 456;

   	yio_stream(yio_pfmt("{:>10}"), "time", " ", yio_pfmt("{:<10}"), "value", "\n");
   	yio_stream(yio_pfmt("{:>10}"), mytime, " ", yio_pfmt("{:<10}"), value, "\n");

   	yio_print("{:>10} {:<10}\n", "time", "value");
   	yio_print("{:>10} {:<10}\n", mytime, value);
}
// PASS_REGULAR_EXPRESSION       time value[ ]*
// PASS_REGULAR_EXPRESSION        123 456[ ]*
// PASS_REGULAR_EXPRESSION       time value[ ]*
// PASS_REGULAR_EXPRESSION        123 456[ ]*
