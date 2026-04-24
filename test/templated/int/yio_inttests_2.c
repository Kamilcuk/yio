/*
 * yio_unittest.c
 *
 *  Created on: 19 pa� 2019
 *      Author: Kamil
 */
#include <yio_test.h>
#include <yio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#if 1 == 1
#define shouldbe_snprintf snprintf
#elif 1 == 2
#define shouldbe_snprintf swprintf
#elif 1 == 3
#define shouldbe_snprintf u16_u16_snprintf
#elif 1 == 4
#define shouldbe_snprintf u32_u32_snprintf
#else
#error
#endif

#define _yio_inttest2(SUFFIX, TYPE) \
static inline \
int _yio_inttest2_ ## SUFFIX(int *err, const char *fmt, \
		const char *printfmt, TYPE arg) { \
	char shouldbe[256]; \
	char buf[256]; \
	shouldbe_snprintf(shouldbe, sizeof(shouldbe)/sizeof(*shouldbe), printfmt, arg); \
	if (1) { \
		fprintf(stdout, "Testing %s yio_print|\"%""s""\", ", #TYPE, fmt); \
		fprintf(stdout, "| \tvs snprintf|\"%""s""\", ", printfmt ? printfmt : "?"); \
		TFPRINTF(stdout, "| \t-> %d\"%""s""\"\n", \
				(int)strlen(shouldbe), shouldbe); \
	} \
	const int ret = yio_sprintf(buf, sizeof(buf), fmt, arg); \
	if (ret < 0) { \
		fprintf(stderr, "%s:%d: yio_print \"%""s""\", ", __FILE__, __LINE__, fmt); \
		fprintf(stderr, " -> %d\n", ret); \
		*err = __LINE__; \
	} \
	if (strcmp(shouldbe, buf) != 0) { \
		fprintf(stderr, "%s:%d: yio_print|\"%""s""\", ", __FILE__, __LINE__, fmt); \
		TFPRINTF(stderr, "| -> %d\"%""s""\" != %d\"%""s""\"\n", \
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
