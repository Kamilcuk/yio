
#include <yio_test.h>

int main() {
#if YIO_VERSION < 0
#error
#endif
#if YIO_VERSION < 0x00010500
#error
#endif
	printf("%#010lx == %#010lx\n", YIO_VERSION, yio_get_version());
	YIO_TESTEXPR(YIO_VERSION == yio_get_version());
	printf("%s == %s\n", YIO_VERSION_STRING, yio_get_version_string());
	YIO_TESTEXPR(strcmp(YIO_VERSION_STRING, yio_get_version_string()) == 0);
}
