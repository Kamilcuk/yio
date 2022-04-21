/*
 * yio_test_8.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>
#include <yπio.h>

static void _dbgln(const char file[], int line, const char func[], yπio_printdata_t *data, const Ychar *fmt, ...) {
	yprintf("{}:{}:{}: ", file, line, func);
	va_list va;
	va_start(va, fmt);
	Ychar * const ret = yπvformatf(data, fmt, &va);
	va_end(va);
	if (ret == NULL) {
		exit(EXIT_FAILURE);
	}
	yprintf("{}", ret);
	free(ret);
	yprintf("\n");
}

#define dbgln(...)  _dbgln(__FILE__, __LINE__, __func__, YΩIO_PRINT_ARGUMENTS(NULL, ##__VA_ARGS__))

int main() {
	dbgln("debugging test1");
	dbgln("debugging ", "test2");
}
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_2.c:[0-9]*:main: debugging test1
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_2.c:[0-9]*:main: debugging test2
