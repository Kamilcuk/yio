/*
 * yio_argtest.c
 *
 *  Created on: 29 gru 2019
 *      Author: Kamil
 */
#include "yio.h"
#include <string.h>

#define YIO_TEST(should_be, fmt, ...)  do { \
	char _tHe_buf[100]; \
	const int _tHe_err = ysprintf(_tHe_buf, sizeof(_tHe_buf), fmt, ## __VA_ARGS__); \
	if (_tHe_err < 0) { \
		fprintf(stderr, "%s:%d: '%s' -> %d\n", \
				__FILE__, __LINE__, fmt, _tHe_err); \
		return -__LINE__; \
	} \
	if (strcmp(should_be, _tHe_buf) != 0) { \
		fprintf(stderr, "%s:%d: '%s' != '%s'\n", \
				__FILE__, __LINE__, should_be, _tHe_buf); \
		return -__LINE__; \
	} \
	printf("%s:%d: note: '%s' == '%s'\n", \
			__FILE__, __LINE__, should_be, _tHe_buf); \
} while(0)

int main() {
	YIO_TEST(" B", " B");
	YIO_TEST(" { } } { { ", " {{ }} }} {{ {{ ");
	YIO_TEST(" 1", " {}", "1");
	YIO_TEST(" A", " {}", "A");
	YIO_TEST(" Hello World! ", " {:}{: 6}{} ", "Hello", "World", (char)'!');
}
