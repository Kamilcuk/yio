#include <yio.h>
int main() {
	double a = 123.456789;
	yio_stream(a, "\n");
	yio_stream(a, "\n");
}
// PASS_REGULAR_EXPRESSION 123.45[0-9]*
// PASS_REGULAR_EXPRESSION 123.45[0-9]*
