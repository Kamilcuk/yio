#include <yio_test.h>
int main() {
	const char out[] = "this is a very long time format string that is longer than 80 characters because I need to test long";
	char *fmt = NULL;
	int err = yaprintf(&fmt, "{{:{}}}", out);
	YIO_TESTEXPR(err > 0, "%s", yio_strerror(err));
	YIO_TESTEXPR(fmt != NULL);
	struct tm t = {0};
	char *str = NULL;
	err = yaprintf(&str, fmt, t);
	YIO_TESTEXPR(err > 0, "%s | %s", yio_strerror(err), fmt);
	YIO_TESTEXPR(str != NULL);
	YIO_TESTEXPR(strcmp(str, out) == 0, "%s | %s", str, fmt);
	yprintf("`{}`\n", str);
	free(fmt);
	free(str);
}
