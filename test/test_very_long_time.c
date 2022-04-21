#include <yio_test.h>
int main() {
	const char out[] = "this is a very long time format string that is longer than 80 characters because I need to test long";
	char *fmt = NULL;
	int err = yaprintf(&fmt, "{{:{}}}", out);
	_yIO_TEST(err > 0, "%s", yio_strerror(err));
	_yIO_TEST(fmt != NULL);
	struct tm t = {0};
	char *str = NULL;
	err = yaprintf(&str, fmt, t);
	_yIO_TEST(err > 0, "%s | %s", yio_strerror(err), fmt);
	_yIO_TEST(str != NULL);
	_yIO_TEST(strcmp(str, out) == 0, "%s | %s", str, fmt);
	yprintf("`{}`\n", str);
	free(fmt);
	free(str);
}
