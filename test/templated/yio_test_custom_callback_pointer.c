/*
 * yio_test_8.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>
#include <yio.h>
#include <stdlib.h>

static int (* const f_vpnt)(char **strp, const yio_printdata_t *, const char *fmt, va_list *va) = yio_vasprintf;
static int (* const f_pnt)(const yio_printdata_t *, const char *fmt, ...) = YIO_yio_print;

static void _dbgln(const char file[], int line, const char func[], const yio_printdata_t *data, const char *fmt, ...) {
	yio_print("{}:{}:{}: ", file, line, func);
	va_list va;
	va_start(va, fmt);
	char *ret = NULL;
	int err = f_vpnt(&ret, data, fmt, &va);
	va_end(va);
	if (err < 0 || ret == NULL) {
		exit(EXIT_FAILURE);
	}
	yio_print("{}", ret);
	free(ret);
	yio_print("\n");
	fflush(stdout);
}

#define dbgln(...)  _dbgln(__FILE__, __LINE__, __func__, YIO_PRINT_ARGUMENTS(NULL, ##__VA_ARGS__))
#define dbgln2(...)  f_pnt(YIO_PRINT_ARGUMENTS(NULL, __FILE__, ":", __LINE__, ":", __func__, ": ", ##__VA_ARGS__, "\n"))

int main() {
	dbgln("debugging test", 1);
	dbgln2("debugging ", "test", 2);
}
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_pointer.c:[0-9]*:main: debugging test1
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_pointer.c:[0-9]*:main: debugging test2
