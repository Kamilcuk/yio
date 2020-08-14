// https://www.programiz.com/c-programming/examples/print-integer
#define _GNU_SOURCE  1
#include <yio.h>
#include <string.h>

static int normal() {
    int number;

    printf("Enter an integer:\n");

    // reads and stores input
    if (scanf("%d", &number) != 1) abort();

    // displays output
    printf("You entered: %d\n", number);

    return 0;
}

static int streamed() {
    int number;

    yprint("Enter an integer:\n");

    // reads and stores input
    if (yscan(&number).nitem != 1) abort();

    // displays output
    yprint("You entered: ", number, "\n");

    return 0;
}

static int formatted() {
    int number;

    yprintf("Enter an integer:\n");

    // reads and stores input
    if (yscanf("{}", &number).nitem != 1) abort();

    // displays output
    yprintf("You entered: {}\n", number);

    return 0;
}

int main() {
	normal();
	fflush(stdout);
	streamed();
	formatted();
	return 0;
}
// INPUT 1 2 3
// PASS_REGULAR_EXPRESSION Enter an integer:
// PASS_REGULAR_EXPRESSION You entered: 1
// PASS_REGULAR_EXPRESSION Enter an integer:
// PASS_REGULAR_EXPRESSION You entered: 2
// PASS_REGULAR_EXPRESSION Enter an integer:
// PASS_REGULAR_EXPRESSION You entered: 3
