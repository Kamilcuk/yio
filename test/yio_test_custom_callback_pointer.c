/*
 * yio_test_8.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>

static char *(* const f_vpnt)(yio_printdata_t *, const char *fmt, va_list *va) = yvformatf;
static int (* const f_pnt)(yio_printdata_t *, const char *fmt, ...) = _yIO_yprintf;

void _dbgln(const char file[], int line, const char func[], yio_printdata_t *data, const char *fmt, ...) {
	printf("%s:%d:%s: ", file, line, func);
	va_list va;
	va_start(va, fmt);
	char * const ret = f_vpnt(data, fmt, &va);
	va_end(va);
	if (ret == NULL) {
		exit(EXIT_FAILURE);
	}
	printf("%s", ret);
	free(ret);
	printf("\n");
	fflush(stdout);
}

#define dbgln(...)  _dbgln(__FILE__, __LINE__, __func__, YIO_PRINT_ARGUMENTS(NULL, ##__VA_ARGS__))
#define dbgln2(...)  f_pnt(YIO_PRINT_ARGUMENTS(NULL, __FILE__, ":", __LINE__, ":", __func__, ": ", ##__VA_ARGS__))

int main() {
	dbgln("debugging test");
	dbgln2("debugging ", "test", "\n");
}
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_pointer.c:[0-9]*:main: debugging test
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_pointer.c:[0-9]*:main: debugging test
