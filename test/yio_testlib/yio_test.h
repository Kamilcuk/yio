
#pragma once
#include <yio.h>
void _yIO__test_failed(const char *expr, const char *file, int line,
		const char *fmt, ...);

#define _yIO_TEST(expr) _yIO_TEST_MSG(expr, "")
#define _yIO_TEST_MSG(expr, str, ...)  do{ \
		if (!(expr)) { \
			_yIO__test_failed(#expr, __FILE__, __LINE__, str, ##__VA_ARGS__); \
		} \
}while(0)
