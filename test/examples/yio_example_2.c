// https://www.programiz.com/c-programming/examples/print-integer
#define _GNU_SOURCE
#include <yio.h>
#include <stdio.h>
#include <string.h>

int normal() {
	int number1, number2, sum;

	printf("Enter two integers:\n");
	if (scanf("%d %d", &number1, &number2) != 2) {
		abort();
	}

	// calculating sum
	sum = number1 + number2;

	printf("%d + %d = %d\n", number1, number2, sum);
	return 0;
}

int streamed() {
	int number1, number2, sum;

	yprint("Enter two integers:\n");
	if (yscan(&number1, &number2).nitem != 2) {
		abort();
	}

	// calculating sum
	sum = number1 + number2;

	yprint(number1, " + ", number2, " = ", sum, "\n");
	return 0;
}

int formatted() {
	int number1, number2, sum;

	yprintf("Enter two integers:\n");
	if (yscanf("{}{}", &number1, &number2).nitem != 2) {
		abort();
	}

	// calculating sum
	sum = number1 + number2;

	yprintf("{} + {} = {}\n", number1, number2, sum);
	return 0;
}

int main() {
	normal();
	fflush(stdout);
	streamed();
	formatted();
	return 0;
}
// INPUT 1 2 3 4 5 6
// PASS_REGULAR_EXPRESSION Enter two integers:
// PASS_REGULAR_EXPRESSION 1 \+ 2 = 3
// PASS_REGULAR_EXPRESSION Enter two integers:
// PASS_REGULAR_EXPRESSION 3 \+ 4 = 7
// PASS_REGULAR_EXPRESSION Enter two integers:
// PASS_REGULAR_EXPRESSION 5 \+ 6 = 11
