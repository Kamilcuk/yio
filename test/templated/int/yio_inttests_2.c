/*
 * yio_unittest.c
 *
 *  Created on: 19 pa� 2019
 *      Author: Kamil
 */
#include <yio_test.h>
#include <yπio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#if TMODE == 1
#define shouldbe_snprintf snprintf
#elif TMODE == 2
#define shouldbe_snprintf swprintf
#elif TMODE == 3
#define shouldbe_snprintf u16_u16_snprintf
#elif TMODE == 4
#define shouldbe_snprintf u32_u32_snprintf
#else
#error
#endif

#define _yio_inttest2(SUFFIX, TYPE) \
static inline \
int _yio_inttest2_ ## SUFFIX(int *err, const TCHAR *fmt, \
		const TCHAR *printfmt, TYPE arg) { \
	TCHAR shouldbe[256]; \
	TCHAR buf[256]; \
	shouldbe_snprintf(shouldbe, sizeof(shouldbe)/sizeof(*shouldbe), printfmt, arg); \
	if (1) { \
		TFPRINTF(stdout, "Testing %s yprint|\"%"TPRI"\", ", #TYPE, fmt); \
		TFPRINTF(stdout, "| \tvs snprintf|\"%"TPRI"\", ", printfmt ? printfmt : TC("?")); \
		TFPRINTF(stdout, "| \t-> %d\"%"TPRI"\"\n", \
				(int)TSTRLEN(shouldbe), shouldbe); \
	} \
	const int ret = yπsprintf(buf, sizeof(buf), fmt, arg); \
	if (ret < 0) { \
		TFPRINTF(stderr, "%s:%d: yprint \"%"TPRI"\", ", __FILE__, __LINE__, fmt); \
		TFPRINTF(stderr, " -> %d\n", ret); \
		*err = __LINE__; \
	} \
	if (TSTRCMP(shouldbe, buf) != 0) { \
		TFPRINTF(stderr, "%s:%d: yprint|\"%"TPRI"\", ", __FILE__, __LINE__, fmt); \
		TFPRINTF(stderr, "| -> %d\"%"TPRI"\" != %d\"%"TPRI"\"\n", \
				(int)TSTRLEN(buf), buf, \
				(int)TSTRLEN(shouldbe), shouldbe); \
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
	_yio_inttest2_int(&err, TC("{}"), TC("%d"), 5);
	_yio_inttest2_int(&err, TC("{}"), TC("%d"), 5);
	_yio_inttest2_int(&err, TC("{}"), TC("%d"), INT_MAX);
	_yio_inttest2_int(&err, TC("{}"), TC("%d"), INT_MIN);
	_yio_inttest2_uint(&err, TC("{}"), TC("%u"), UINT_MAX);
	_yio_inttest2_short(&err, TC("{}"), TC("%hd"), SHRT_MIN);
	_yio_inttest2_short(&err, TC("{}"), TC("%hd"), SHRT_MAX);
	_yio_inttest2_ushort(&err, TC("{}"), TC("%hu"), USHRT_MAX);
	_yio_inttest2_long(&err, TC("{}"), TC("%ld"), LONG_MIN);
	_yio_inttest2_long(&err, TC("{}"), TC("%ld"), LONG_MAX);
	_yio_inttest2_ulong(&err, TC("{}"), TC("%lu"), ULONG_MAX);
	_yio_inttest2_llong(&err, TC("{}"), TC("%lld"), LLONG_MIN);
	_yio_inttest2_llong(&err, TC("{}"), TC("%lld"), LLONG_MAX);
	_yio_inttest2_ullong(&err, TC("{}"), TC("%llu"), ULLONG_MAX);
	_yio_inttest2_ullong(&err, TC("{: 30}"), TC("% 30llu"), ULLONG_MAX);
	_yio_inttest2_ullong(&err, TC("{:#030X}"), TC("%+#030llX"), ULLONG_MAX);

	return err;
}
