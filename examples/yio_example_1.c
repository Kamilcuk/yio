// https://www.programiz.com/c-programming/examples/print-integer
#include <yio.h>

static void normal() {
    int number = 1;
    printf("number: %d\n", number);
}

static void streamed() {
    int number = 2;
    yio_stream("number: ", number, "\n");
}

static void formatted() {
    int number = 3;
    yio_printf("number: {}\n", number);
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
