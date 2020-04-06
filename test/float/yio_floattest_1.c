#include <yio.h>
int main() {
	float arg = 123.456789;
	yprintf("{}", arg);
}
// PASS_REGULAR_EXPRESSION 123.45678[0-9]+
