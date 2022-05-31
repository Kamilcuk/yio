
#include <yio_test.h>

int main() {
#if YIO_VERSION < 0
#error
#endif
#if YIO_VERSION < 0x00010500
#error
#endif
	printf("%#010lx == %#010lx\n", YIO_VERSION, yio_get_version());
	YYIO_TEST(YIO_VERSION == yio_get_version());
	printf("%s == %s\n", YIO_VERSION_STRING, yio_get_version_string());
	YYIO_TEST(strcmp(YIO_VERSION_STRING, yio_get_version_string()) == 0);
}
