/*
 * yio_test_8.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>
#include <yπio.h>
#include <stdlib.h>

static TCHAR *(* const f_vpnt)(yπio_printdata_t *, const TCHAR *fmt, va_list *va) = yπvformatf;
static int (* const f_pnt)(yπio_printdata_t *, const TCHAR *fmt, ...) = YYΩIO_yprintf;

static void _dbgln(const char file[], int line, const char func[], yπio_printdata_t *data, const TCHAR *fmt, ...) {
	yprintf("{}:{}:{}: ", file, line, func);
	va_list va;
	va_start(va, fmt);
	TCHAR * const ret = f_vpnt(data, fmt, &va);
	va_end(va);
	if (ret == NULL) {
		exit(EXIT_FAILURE);
	}
	yprintf("{}", ret);
	free(ret);
	yprintf("\n");
	fflush(stdout);
}

#define dbgln(...)  _dbgln(__FILE__, __LINE__, __func__, YΩIO_PRINT_ARGUMENTS(NULL, ##__VA_ARGS__))
#define dbgln2(...)  f_pnt(YΩIO_PRINT_ARGUMENTS(NULL, __FILE__, ":", __LINE__, ":", __func__, ": ", ##__VA_ARGS__, "\n"))

int main() {
	dbgln("debugging test", 1);
	dbgln2("debugging ", "test", 2);
}
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_pointer.c:[0-9]*:main: debugging test1
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_pointer.c:[0-9]*:main: debugging test2
