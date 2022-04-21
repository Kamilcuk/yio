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
	yprint("Character is ", ch, "\n");
	yprint("String is ", str, "\n");
	yprint("Float value is ", yppfmt("{:f}"), flt, "\n");
	yprint("Integer value is ", no, "\n");
	yprint("Double value is ", yppfmt("{:f}"), dbl, "\n");
	yprint("Octal value is ", yppfmt("{:o}"), no, "\n");
	yprint("Octal value is ", yppfmt("{:o}"), no, "\n");
	yprint("Hexadecimal value is ", yppfmt("{:x}"), no, "\n");
	yprint("Hexadecimal value is ", yppfmt("{:x}"), no, "\n");
}

static void formatted() {
	char ch = 'A';
	char str[20] = "fresh2refresh.com";
	float flt = 10.234;
	int no = 150;
	double dbl = 20.123456;
	yprintf("Character is {}\n", ch);
	yprintf("String is {}\n", str);
	yprintf("Float value is {:f}\n", flt);
	yprintf("Integer value is {}\n", no);
	yprintf("Double value is {:f}\n", dbl);
	yprintf("Octal value is {:o}\n", no);
	yprintf("Hexadecimal value is {:x}\n", no);
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
