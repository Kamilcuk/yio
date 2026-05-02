#include <yio.h>
int main() {
	yio_stream("Hello world\n");
	yio_stream("Hello world\n");
}
// PASS_REGULAR_EXPRESSION Hello world
// PASS_REGULAR_EXPRESSION Hello world
