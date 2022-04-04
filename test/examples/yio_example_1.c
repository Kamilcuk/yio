// https://www.programiz.com/c-programming/examples/print-integer
#define _GNU_SOURCE  1
#include <yio.h>
#include <string.h>

static int normal() {
    int number = 1;
    printf("number: %d\n", number);
    return 0;
}

static int streamed() {
    int number = 2;
    yprint("number: ", number, "\n");
    return 0;
}

static int formatted() {
    int number = 3;
    yprintf("number: {}\n", number);
    return 0;
}

int main() {
	normal();
	fflush(stdout);
	streamed();
	formatted();
	return 0;
}
// PASS_REGULAR_EXPRESSION number: 1
// PASS_REGULAR_EXPRESSION number: 2
// PASS_REGULAR_EXPRESSION number: 3
