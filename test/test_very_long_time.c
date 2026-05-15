#include <yio_test.h>
int main() {
#if YIO_ENABLE_MALLOC
	const char out[] = "this is a very long time format string that is longer than 80 characters because I need to test long";
	char *fmt = NULL;
	int err = yio_asprint(&fmt, "{{:{}}}", out);
	YIO_TESTEXPR(err > 0, "%s", yio_strerror(err));
	YIO_TESTEXPR(fmt != NULL);
	struct tm t = {0};
	char *str = NULL;
	err = yio_asprint(&str, fmt, t);
	YIO_TESTEXPR(err > 0, "%s | %s", yio_strerror(err), fmt);
	YIO_TESTEXPR(str != NULL);
	YIO_TESTEXPR(strcmp(str, out) == 0, "%s | %s", str, fmt);
	yio_print("`{}`\n", str);
	free(fmt);
	free(str);
#endif
	return 0;
}
