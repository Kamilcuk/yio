/*
 * yio_unittest.c
 *
 *  Created on: 19 pa� 2019
 *      Author: Kamil
 */
#include <yπio.h>
#include <string.h>
#if _yIO_TYPE_YUIO
#include <unistdio.h>
#include <unistr.h>
#endif

struct _yIO_inttests_s {
	const Ychar *fmt;
	int arg;
	const Ychar *output;
};

static const struct _yIO_inttests_s _yIO_inttests[] = {
		{ Yc("A{}"), 5, Yc("A5"), },
		{ Yc("A{:}"), 5, Yc("A5"), },

		{ Yc("A{:d}"), 5, Yc("A5"), },
		{ Yc("A{:x}"), 5, Yc("A5"), },
		{ Yc("A{:X}"), 5, Yc("A5"), },

		{ Yc("A{:x}"), 0xa, Yc("Aa"), },
		{ Yc("A{:X}"), 0xa, Yc("AA"), },
		{ Yc("A{:o}"), 077, Yc("A77"), },

		{ Yc("A{:#o}"), 077, Yc("A0o77"), },
		{ Yc("A{:#x}"), 0xa, Yc("A0xa"), },
		{ Yc("A{:#X}"), 0xA, Yc("A0XA"), },

		{ Yc("A{:010d}"), 5, Yc("A0000000005"), },

		{ Yc("B{:>10d}"),  5, Yc("B         5"), },
		{ Yc("B{:^10d}"),  5, Yc("B     5    "), },
		{ Yc("B{:<10d}"),  5, Yc("B5         "), },
		{ Yc("B{:=10d}"),  5, Yc("B         5"), },

		{ Yc("B{:>+10d}"), 5, Yc("B        +5"), },
		{ Yc("B{:^+10d}"), 5, Yc("B    +5    "), },
		{ Yc("B{:<+10d}"), 5, Yc("B+5        "), },
		{ Yc("B{:=+10d}"), 5, Yc("B+        5"), },

		{ Yc("B{:>-10d}"), 5, Yc("B         5"), },
		{ Yc("B{:^-10d}"), 5, Yc("B     5    "), },
		{ Yc("B{:<-10d}"), 5, Yc("B5         "), },
		{ Yc("B{:=-10d}"), 5, Yc("B         5"), },

		{ Yc("B{:> 10d}"), 5, Yc("B         5"), },
		{ Yc("B{:^ 10d}"), 5, Yc("B     5    "), },
		{ Yc("B{:< 10d}"), 5, Yc("B 5        "), },
		{ Yc("B{:= 10d}"), 5, Yc("B         5"), },

		{ Yc("C{:>10d}"), -5, Yc("C        -5"), },
		{ Yc("C{:^10d}"), -5, Yc("C    -5    "), },
		{ Yc("C{:<10d}"), -5, Yc("C-5        "), },
		{ Yc("C{:=10d}"), -5, Yc("C-        5"), },

		{ Yc("C{:>+10x}"), -5, Yc("C        -5"), },
		{ Yc("C{:^+10x}"), -5, Yc("C    -5    "), },
		{ Yc("C{:<+10x}"), -5, Yc("C-5        "), },
		{ Yc("C{:=+10x}"), -5, Yc("C-        5"), },

		{ Yc("C{:>-10d}"), -5, Yc("C        -5"), },
		{ Yc("C{:^-10d}"), -5, Yc("C    -5    "), },
		{ Yc("C{:<-10d}"), -5, Yc("C-5        "), },
		{ Yc("C{:=-10d}"), -5, Yc("C-        5"), },

		{ Yc("D{:> 10x}"), -5, Yc("D        -5"), },
		{ Yc("D{:^ 10x}"), -5, Yc("D    -5    "), },
		{ Yc("D{:< 10x}"), -5, Yc("D-5        "), },
		{ Yc("D{:= 10x}"), -5, Yc("D-        5"), },

		{ Yc("D{:>+10x}"), -5, Yc("D        -5"), },
		{ Yc("D{:^+10x}"), -5, Yc("D    -5    "), },
		{ Yc("D{:<+10x}"), -5, Yc("D-5        "), },
		{ Yc("D{:=+10x}"), -5, Yc("D-        5"), },

		{ Yc("D{:>-10x}"), -5, Yc("D        -5"), },
		{ Yc("D{:^-10x}"), -5, Yc("D    -5    "), },
		{ Yc("D{:<-10x}"), -5, Yc("D-5        "), },
		{ Yc("D{:=-10x}"), -5, Yc("D-        5"), },

		{ Yc("E{:>10}"),  5, Yc("E         5"), },
		{ Yc("E{:^10}"),  5, Yc("E     5    "), },
		{ Yc("E{:<10}"),  5, Yc("E5         "), },
		{ Yc("E{:=10}"),  5, Yc("E         5"), },

		{ Yc("E{:>+10}"), 5, Yc("E        +5"), },
		{ Yc("E{:^+10}"), 5, Yc("E    +5    "), },
		{ Yc("E{:<+10}"), 5, Yc("E+5        "), },
		{ Yc("E{:=+10}"), 5, Yc("E+        5"), },

		{ Yc("E{:>-10}"), 5, Yc("E         5"), },
		{ Yc("E{:^-10}"), 5, Yc("E     5    "), },
		{ Yc("E{:<-10}"), 5, Yc("E5         "), },
		{ Yc("E{:=-10}"), 5, Yc("E         5"), },

		{ Yc("E{:> 10}"), 5, Yc("E         5"), },
		{ Yc("E{:^ 10}"), 5, Yc("E     5    "), },
		{ Yc("E{:< 10}"), 5, Yc("E 5        "), },
		{ Yc("E{:= 10}"), 5, Yc("E         5"), },

		{ Yc("F{:>#10x}"), -5, Yc("F      -0x5"), },
		{ Yc("F{:^#10x}"), -5, Yc("F   -0x5   "), },
		{ Yc("F{:<#10x}"), -5, Yc("F-0x5      "), },
		{ Yc("F{:=#10x}"), -5, Yc("F-0x      5"), },

		{ Yc("F{:>+#10x}"), -5, Yc("F      -0x5"), },
		{ Yc("F{:^+#10x}"), -5, Yc("F   -0x5   "), },
		{ Yc("F{:<+#10x}"), -5, Yc("F-0x5      "), },
		{ Yc("F{:=+#10x}"), -5, Yc("F-0x      5"), },

		{ Yc("F{:>-#10x}"), -5, Yc("F      -0x5"), },
		{ Yc("F{:^-#10x}"), -5, Yc("F   -0x5   "), },
		{ Yc("F{:<-#10x}"), -5, Yc("F-0x5      "), },
		{ Yc("F{:=-#10x}"), -5, Yc("F-0x      5"), },

		{ Yc("F{:> #10x}"), -5, Yc("F      -0x5"), },
		{ Yc("F{:^ #10x}"), -5, Yc("F   -0x5   "), },
		{ Yc("F{:< #10x}"), -5, Yc("F-0x5      "), },
		{ Yc("F{:= #10x}"), -5, Yc("F-0x      5"), },

		{ Yc("G{:>#10x}"), 5, Yc("G       0x5"), },
		{ Yc("G{:^#10x}"), 5, Yc("G    0x5   "), },
		{ Yc("G{:<#10x}"), 5, Yc("G0x5       "), },
		{ Yc("G{:=#10x}"), 5, Yc("G0x       5"), },

		{ Yc("G{:>+#10x}"), 5, Yc("G      +0x5"), },
		{ Yc("G{:^+#10x}"), 5, Yc("G   +0x5   "), },
		{ Yc("G{:<+#10x}"), 5, Yc("G+0x5      "), },
		{ Yc("G{:=+#10x}"), 5, Yc("G+0x      5"), },

		{ Yc("G{:>-#10x}"), 5, Yc("G       0x5"), },
		{ Yc("G{:^-#10x}"), 5, Yc("G    0x5   "), },
		{ Yc("G{:<-#10x}"), 5, Yc("G0x5       "), },
		{ Yc("G{:=-#10x}"), 5, Yc("G0x       5"), },

		{ Yc("G{:> #10x}"), 5, Yc("G       0x5"), },
		{ Yc("G{:^ #10x}"), 5, Yc("G    0x5   "), },
		{ Yc("G{:< #10x}"), 5, Yc("G 0x5      "), },
		{ Yc("G{:= #10x}"), 5, Yc("G 0x      5"), },

		{ Yc("H{:@> #10x}"), 5, Yc("H@@@@@@ 0x5"), },
		{ Yc("H{:@^ #10x}"), 5, Yc("H@@@ 0x5@@@"), },
		{ Yc("H{:@< #10x}"), 5, Yc("H 0x5@@@@@@"), },
		{ Yc("H{:@= #10x}"), 5, Yc("H 0x@@@@@@5"), },

		{ Yc("H{:\x02> #10x}"), 5, Yc("H\x02\x02\x02\x02\x02\x02 0x5"), },
		{ Yc("H{:\x02^ #10x}"), 5, Yc("H\x02\x02\x02 0x5\x02\x02\x02"), },
		{ Yc("H{:\x02< #10x}"), 5, Yc("H 0x5\x02\x02\x02\x02\x02\x02"), },
		{ Yc("H{:\x02= #10x}"), 5, Yc("H 0x\x02\x02\x02\x02\x02\x02""5"), },

};

#define _yIO_inttest_DECL(suffix, type) \
\
static inline \
int _yIO_inttest_ ## suffix(void) { \
	int err = 0; \
	Ychar buf[256]; \
	for (size_t i = 0; i < sizeof(_yIO_inttests)/sizeof(_yIO_inttests[0]); ++i) { \
 \
	 	 if ((type)-1 > 0 && _yIO_inttests[i].arg < 0) continue; \
						\
		memset(buf, 0, sizeof(buf)); \
		if (1) { \
			Yfprintf(stderr, "Testing yprintf(\"%"YPRI"s\", %d) -> %d\"%"YPRI"s\"\n", \
					_yIO_inttests[i].fmt, _yIO_inttests[i].arg, \
					(int)Ystrlen(_yIO_inttests[i].output), _yIO_inttests[i].output); \
		} \
 \
		const int ret = yπsprintf(buf, sizeof(buf), _yIO_inttests[i].fmt, (type)_yIO_inttests[i].arg); \
 \
		if (ret < 0) { \
			Yfprintf(stderr, "%s:%d: %d: printf(\"%"YPRI"s\", %d) -> %d\n", \
					__FILE__, __LINE__, (int)i, \
					_yIO_inttests[i].fmt, _yIO_inttests[i].arg, ret); \
			err = i + 1; \
		} \
		if (Ystrcmp(buf, _yIO_inttests[i].output) != 0) { \
			Yfprintf(stderr, "%s:%d: %d: printf(\"%"YPRI"s\", %d) -> %d\"%"YPRI"s\" should be %d\"%"YPRI"s\"\n", \
					__FILE__, __LINE__, (int)i, \
					_yIO_inttests[i].fmt, _yIO_inttests[i].arg, \
					(int)Ystrlen(buf), buf, \
					(int)Ystrlen(_yIO_inttests[i].output), _yIO_inttests[i].output); \
			err = i + 1; \
		} \
 \
	} \
	return err; \
}

_yIO_inttest_DECL(short, short)
_yIO_inttest_DECL(ushort, unsigned short)
_yIO_inttest_DECL(int, int)
_yIO_inttest_DECL(uint, unsigned int)
_yIO_inttest_DECL(long, long)
_yIO_inttest_DECL(ulong, unsigned long)
_yIO_inttest_DECL(llong, long long)
_yIO_inttest_DECL(ullong, unsigned long long)

#define _yIO_inttest_DO(suffix) \
		err = _yIO_inttest_ ## suffix(); \
		if (err) return err;

int main() {
	int err = 0;
	_yIO_inttest_DO(int);
	_yIO_inttest_DO(uint);
	_yIO_inttest_DO(short);
	_yIO_inttest_DO(ushort);
	_yIO_inttest_DO(long);
	_yIO_inttest_DO(ulong);
	_yIO_inttest_DO(llong);
	_yIO_inttest_DO(ullong);
	return 0;
}
