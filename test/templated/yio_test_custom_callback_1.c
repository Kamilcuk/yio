/*
 * yio_test_8.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yπio.h>
#define dbgln(...)  yπprint(__FILE__, ":", __LINE__, ":", __func__, ": ", ##__VA_ARGS__, "\n")
int main() {
	dbgln("debugging test1");
	dbgln("debugging ", "test2");
}
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_1.c:[0-9]*:main: debugging test1
// PASS_REGULAR_EXPRESSION .*yio_test_custom_callback_1.c:[0-9]*:main: debugging test2
