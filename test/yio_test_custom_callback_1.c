/*
 * yio_test_8.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>
#define dbgln(...)  yprint(__FILE__, ":", __LINE__, ":", __func__, ": ", ##__VA_ARGS__, "\n")
int main() {
	dbgln("debugging test");
	dbgln("debugging ", "test");
}
// PASS_REGULAR_EXPRESSION .*/yio_test_custom_callback_1.c:10:main: debugging test
// PASS_REGULAR_EXPRESSION .*/yio_test_custom_callback_1.c:11:main: debugging test
