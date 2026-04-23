#include <yio_test.h>
#include <yio.h>
int main() {
	float arg = 123.456789;
	YIO_TEST_RGX("123.45[67][0-9]*", "{}", arg);
	YIO_TEST_RGX("123.45[67][0-9]*", "{}", arg);
	YIO_TEST_RGX("123.45[67][0-9]*", "{}", arg);
}
