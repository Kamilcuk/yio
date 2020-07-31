/*
 * yio_test_8.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>

void _dbgln(const char file[], int line, const char func[], yio_printdata_t *data, ...) {
	printf("%s:%d:%s: ", file, line, func);
	va_list va;
	va_start(va, data);
	char * const ret = yvformatf(data, &va);
	va_end(va);
	if (ret == NULL) {
		exit(EXIT_FAILURE);
	}
	printf("%s", ret);
	free(ret);
	printf("\n");
}

#define dbgln(...)  _dbgln(__FILE__, __LINE__, __func__, YIO_PRINT_ARGUMENTS(NULL, ##__VA_ARGS__))

int main() {
	dbgln("debugging test");
	dbgln("debugging ", "test");
}
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_2.c:[0-9]*:main: debugging test
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_2.c:[0-9]*:main: debugging test
