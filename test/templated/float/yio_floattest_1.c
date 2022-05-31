#include <yio_test.h>
#include <yπio.h>
int main() {
	float arg = 123.456789;
	YΩIO_TEST("123.45[67][0-9]*", "{}", arg);
	YΩIO_TEST("123.45[67][0-9]*", "{}", arg);
	YΩIO_TEST("123.45[67][0-9]*", "{}", arg);
}
