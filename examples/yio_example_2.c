// https://fresh2refresh.com/c-programming/c-printf-and-scanf/
#include <yio.h>

static void normal() {
	char ch = 'A';
	char str[20] = "fresh2refresh.com";
	float flt = 10.234;
	int no = 150;
	double dbl = 20.123456;
	printf("Character is %c\n", ch);
	printf("String is %s\n", str);
	printf("Float value is %f\n", flt);
	printf("Integer value is %d\n", no);
	printf("Double value is %lf\n", dbl);
	printf("Octal value is %o\n", no);
	printf("Hexadecimal value is %x\n", no);
}

static void streamed() {
	char ch = 'A';
	char str[20] = "fresh2refresh.com";
	float flt = 10.234;
	int no = 150;
	double dbl = 20.123456;
	yio_stream("Character is ", ch, "\n");
	yio_stream("String is ", str, "\n");
	yio_stream("Float value is ", yio_pfmt("{:f}"), flt, "\n");
	yio_stream("Integer value is ", no, "\n");
	yio_stream("Double value is ", yio_pfmt("{:f}"), dbl, "\n");
	yio_stream("Octal value is ", yio_pfmt("{:o}"), no, "\n");
	yio_stream("Octal value is ", yio_pfmt("{:o}"), no, "\n");
	yio_stream("Hexadecimal value is ", yio_pfmt("{:x}"), no, "\n");
	yio_stream("Hexadecimal value is ", yio_pfmt("{:x}"), no, "\n");
}

static void formatted() {
	char ch = 'A';
	char str[20] = "fresh2refresh.com";
	float flt = 10.234;
	int no = 150;
	double dbl = 20.123456;
	yio_print("Character is {}\n", ch);
	yio_print("String is {}\n", str);
	yio_print("Float value is {:f}\n", flt);
	yio_print("Integer value is {}\n", no);
	yio_print("Double value is {:f}\n", dbl);
	yio_print("Octal value is {:o}\n", no);
	yio_print("Hexadecimal value is {:x}\n", no);
}

int main() {
	normal();
	fflush(stdout);
	streamed();
	formatted();
	return 0;
}
// PASS_REGULAR_EXPRESSION Character is A
// PASS_REGULAR_EXPRESSION String is fresh2refresh.com
// PASS_REGULAR_EXPRESSION Float value is 10.234000
// PASS_REGULAR_EXPRESSION Integer value is 150
// PASS_REGULAR_EXPRESSION Double value is 20.123456
// PASS_REGULAR_EXPRESSION Octal value is 226
// PASS_REGULAR_EXPRESSION Hexadecimal value is 96
// PASS_REGULAR_EXPRESSION Character is A
// PASS_REGULAR_EXPRESSION String is fresh2refresh.com
// PASS_REGULAR_EXPRESSION Float value is 10.234000
// PASS_REGULAR_EXPRESSION Integer value is 150
// PASS_REGULAR_EXPRESSION Double value is 20.123456
// PASS_REGULAR_EXPRESSION Octal value is 226
// PASS_REGULAR_EXPRESSION Octal value is 226
// PASS_REGULAR_EXPRESSION Hexadecimal value is 96
// PASS_REGULAR_EXPRESSION Hexadecimal value is 96
// PASS_REGULAR_EXPRESSION Character is A
// PASS_REGULAR_EXPRESSION String is fresh2refresh.com
// PASS_REGULAR_EXPRESSION Float value is 10.234000
// PASS_REGULAR_EXPRESSION Integer value is 150
// PASS_REGULAR_EXPRESSION Double value is 20.123456
// PASS_REGULAR_EXPRESSION Octal value is 226
// PASS_REGULAR_EXPRESSION Hexadecimal value is 96
