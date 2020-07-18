/*
 * yio_unittest.c
 *
 *  Created on: 19 pa� 2019
 *      Author: Kamil
 */
#include <yio/yio_private.h>
#include <yio.h>
#include <string.h>

#define _yio_inttest2(SUFFIX, TYPE) \
static inline \
int _yio_inttest2_ ## SUFFIX(int *err, const char *fmt, \
		const char *printfmt, TYPE arg) { \
	char shouldbe[256]; \
	char buf[256]; \
	snprintf(shouldbe, sizeof(shouldbe), printfmt, arg); \
	if (1) { \
		printf("Testing %s yprint(\"%s\", ", #TYPE, fmt); \
		printf(printfmt, arg); \
		printf(") against snprintf(\"%s\", ", printfmt); \
		printf(printfmt, arg); \
		printf(") should be %d\"%s\"\n", \
				(int)strlen(shouldbe), shouldbe); \
	} \
	const int ret = ysprintf(buf, sizeof(buf), fmt, arg); \
	if (ret < 0) { \
		fprintf(stderr, "%s:%d: yprint(\"%s\", ", __FILE__, __LINE__, fmt); \
		fprintf(stderr, printfmt, arg); \
		fprintf(stderr, " -> %d\n", ret); \
		*err = __LINE__; \
	} \
	if (strcmp(shouldbe, buf) != 0) { \
		fprintf(stderr, "%s:%d: yprint(\"%s\", ", __FILE__, __LINE__, fmt); \
		fprintf(stderr, printfmt, arg); \
		fprintf(stderr, ") -> %d\"%s\" != %d\"%s\"\n", \
				(int)strlen(buf), buf, \
				(int)strlen(shouldbe), shouldbe); \
		*err = __LINE__; \
	} \
	return 0; \
}

_yio_inttest2(int, int)
_yio_inttest2(uint, unsigned)
_yio_inttest2(short, short)
_yio_inttest2(ushort, unsigned short)
_yio_inttest2(long, long)
_yio_inttest2(ulong, unsigned long)
_yio_inttest2(llong, long long)
_yio_inttest2(ullong, unsigned long long)

int main() {
	int err = 0;
	_yio_inttest2_int(&err, "{}", "%d", 5);
	_yio_inttest2_int(&err, "{}", "%d", 5);
	_yio_inttest2_int(&err, "{}", "%d", INT_MAX);
	_yio_inttest2_int(&err, "{}", "%d", INT_MIN);
	_yio_inttest2_uint(&err, "{}", "%u", UINT_MAX);
	_yio_inttest2_short(&err, "{}", "%hd", SHRT_MIN);
	_yio_inttest2_short(&err, "{}", "%hd", SHRT_MAX);
	_yio_inttest2_ushort(&err, "{}", "%hu", USHRT_MAX);
	_yio_inttest2_long(&err, "{}", "%ld", LONG_MIN);
	_yio_inttest2_long(&err, "{}", "%ld", LONG_MAX);
	_yio_inttest2_ulong(&err, "{}", "%lu", ULONG_MAX);
	_yio_inttest2_llong(&err, "{}", "%lld", LLONG_MIN);
	_yio_inttest2_llong(&err, "{}", "%lld", LLONG_MAX);
	_yio_inttest2_ullong(&err, "{}", "%llu", ULLONG_MAX);

	_yio_inttest2_ullong(&err, "{: 30}", "% 30llu", ULLONG_MAX);
	_yio_inttest2_ullong(&err, "{:#030X}", "%+#030llX", ULLONG_MAX);

	return err;
}
