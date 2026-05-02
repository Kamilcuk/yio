/*
 * yio_test_8.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>
#include <yio.h>
#include <stdlib.h>

#if YIO_ENABLE_MALLOC
static void _dbgln(const char file[], int line, const char func[], const yio_printdata_t *data, const char *fmt, ...) {
	yio_printf("{}:{}:{}: ", file, line, func);
	va_list va;
	va_start(va, fmt);
	char *ret = NULL;
	int err = yio_vasprintf(&ret, data, fmt, &va);
	va_end(va);
	if (err < 0 || ret == NULL) {
		exit(EXIT_FAILURE);
	}
	yio_printf("{}", ret);
	free(ret);
	yio_printf("\n");
}

#define dbgln(...)  _dbgln(__FILE__, __LINE__, __func__, YIO_PRINT_ARGUMENTS(NULL, ##__VA_ARGS__))
#endif

int main() {
#if YIO_ENABLE_MALLOC
	dbgln("debugging test1");
	dbgln("debugging ", "test2");
#endif
	return 0;
}
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_2.c:[0-9]*:main: debugging test1
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_2.c:[0-9]*:main: debugging test2
