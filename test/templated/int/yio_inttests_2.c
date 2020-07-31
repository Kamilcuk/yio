/*
 * yio_unittest.c
 *
 *  Created on: 19 pa� 2019
 *      Author: Kamil
 */
#include <yio/yio_private.h>
#include <yio.h>
#include <ywio.h>
#include <string.h>
#include <assert.h>

#if _yIO_TYPE_YIO
#define shouldbe_snprintf snprintf
#elif _yIO_TYPE_YWIO
#define shouldbe_snprintf swprintf
#else
#define shouldbe_snprintf u32_vsnprintf
#endif

#define _yio_inttest2(SUFFIX, TYPE) \
static inline \
int _yio_inttest2_ ## SUFFIX(int *err, const Ychar *fmt, \
		const Ychar *printfmt, TYPE arg) { \
	Ychar shouldbe[256]; \
	Ychar buf[256]; \
	shouldbe_snprintf(shouldbe, sizeof(shouldbe)/sizeof(*shouldbe), printfmt, arg); \
	if (1) { \
		Yprintf("Testing %s yprint|\"%"YPRI"\", ", #TYPE, fmt); \
		Yprintf("| \tvs snprintf|\"%"YPRI"\", ", printfmt ? printfmt : Yc("?")); \
		Yprintf("| \t-> %d\"%"YPRI"\"\n", \
				(int)Ystrlen(shouldbe), shouldbe); \
	} \
	const int ret = yπsprintf(buf, sizeof(buf), fmt, arg); \
	if (ret < 0) { \
		Yfprintf(stderr, "%s:%d: yprint \"%"YPRI"\", ", __FILE__, __LINE__, fmt); \
		Yfprintf(stderr, " -> %d\n", ret); \
		*err = __LINE__; \
	} \
	if (Ystrcmp(shouldbe, buf) != 0) { \
		Yfprintf(stderr, "%s:%d: yprint|\"%"YPRI"\", ", __FILE__, __LINE__, fmt); \
		Yfprintf(stderr, "| -> %d\"%"YPRI"\" != %d\"%"YPRI"\"\n", \
				(int)Ystrlen(buf), buf, \
				(int)Ystrlen(shouldbe), shouldbe); \
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
	_yio_inttest2_int(&err, Yc("{}"), Yc("%d"), 5);
	_yio_inttest2_int(&err, Yc("{}"), Yc("%d"), 5);
	_yio_inttest2_int(&err, Yc("{}"), Yc("%d"), INT_MAX);
	_yio_inttest2_int(&err, Yc("{}"), Yc("%d"), INT_MIN);
	_yio_inttest2_uint(&err, Yc("{}"), Yc("%u"), UINT_MAX);
	_yio_inttest2_short(&err, Yc("{}"), Yc("%hd"), SHRT_MIN);
	_yio_inttest2_short(&err, Yc("{}"), Yc("%hd"), SHRT_MAX);
	_yio_inttest2_ushort(&err, Yc("{}"), Yc("%hu"), USHRT_MAX);
	_yio_inttest2_long(&err, Yc("{}"), Yc("%ld"), LONG_MIN);
	_yio_inttest2_long(&err, Yc("{}"), Yc("%ld"), LONG_MAX);
	_yio_inttest2_ulong(&err, Yc("{}"), Yc("%lu"), ULONG_MAX);
	_yio_inttest2_llong(&err, Yc("{}"), Yc("%lld"), LLONG_MIN);
	_yio_inttest2_llong(&err, Yc("{}"), Yc("%lld"), LLONG_MAX);
	_yio_inttest2_ullong(&err, Yc("{}"), Yc("%llu"), ULLONG_MAX);

	_yio_inttest2_ullong(&err, Yc("{: 30}"), Yc("% 30llu"), ULLONG_MAX);
	_yio_inttest2_ullong(&err, Yc("{:#030X}"), Yc("%+#030llX"), ULLONG_MAX);

	return err;
}
