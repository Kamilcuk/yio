/*
 * yio_unittest.c
 *
 *  Created on: 19 pa� 2019
 *      Author: Kamil
 */
#include <yio_test.h>
#include <string.h>

struct _yIO_inttests_s {
	const TCHAR *fmt;
	int arg;
	const TCHAR *output;
};

static const struct _yIO_inttests_s _yIO_inttests[] = {
		{ TC("A{}"), 5, TC("A5"), },
		{ TC("A{:}"), 5, TC("A5"), },

		{ TC("A{:d}"), 5, TC("A5"), },
		{ TC("A{:x}"), 5, TC("A5"), },
		{ TC("A{:X}"), 5, TC("A5"), },

		{ TC("A{:x}"), 0xa, TC("Aa"), },
		{ TC("A{:X}"), 0xa, TC("AA"), },
		{ TC("A{:o}"), 077, TC("A77"), },

		{ TC("A{:#o}"), 077, TC("A0o77"), },
		{ TC("A{:#x}"), 0xa, TC("A0xa"), },
		{ TC("A{:#X}"), 0xA, TC("A0XA"), },

		{ TC("A{:010d}"), 5, TC("A0000000005"), },

		{ TC("B{:>10d}"),  5, TC("B         5"), },
		{ TC("B{:^10d}"),  5, TC("B    5     "), },
		{ TC("B{:<10d}"),  5, TC("B5         "), },
		{ TC("B{:=10d}"),  5, TC("B         5"), },

		{ TC("B{:>+10d}"), 5, TC("B        +5"), },
		{ TC("B{:^+10d}"), 5, TC("B    +5    "), },
		{ TC("B{:<+10d}"), 5, TC("B+5        "), },
		{ TC("B{:=+10d}"), 5, TC("B+        5"), },

		{ TC("B{:>-10d}"), 5, TC("B         5"), },
		{ TC("B{:^-10d}"), 5, TC("B    5     "), },
		{ TC("B{:<-10d}"), 5, TC("B5         "), },
		{ TC("B{:=-10d}"), 5, TC("B         5"), },

		{ TC("B{:> 10d}"), 5, TC("B         5"), },
		{ TC("B{:^ 10d}"), 5, TC("B     5    "), },
		{ TC("B{:< 10d}"), 5, TC("B 5        "), },
		{ TC("B{:= 10d}"), 5, TC("B         5"), },

		{ TC("C{:>10d}"), -5, TC("C        -5"), },
		{ TC("C{:^10d}"), -5, TC("C    -5    "), },
		{ TC("C{:<10d}"), -5, TC("C-5        "), },
		{ TC("C{:=10d}"), -5, TC("C-        5"), },

		{ TC("C{:>+10x}"), -5, TC("C        -5"), },
		{ TC("C{:^+10x}"), -5, TC("C    -5    "), },
		{ TC("C{:<+10x}"), -5, TC("C-5        "), },
		{ TC("C{:=+10x}"), -5, TC("C-        5"), },

		{ TC("C{:>-10d}"), -5, TC("C        -5"), },
		{ TC("C{:^-10d}"), -5, TC("C    -5    "), },
		{ TC("C{:<-10d}"), -5, TC("C-5        "), },
		{ TC("C{:=-10d}"), -5, TC("C-        5"), },

		{ TC("D{:> 10x}"), -5, TC("D        -5"), },
		{ TC("D{:^ 10x}"), -5, TC("D    -5    "), },
		{ TC("D{:< 10x}"), -5, TC("D-5        "), },
		{ TC("D{:= 10x}"), -5, TC("D-        5"), },

		{ TC("D{:>+10x}"), -5, TC("D        -5"), },
		{ TC("D{:^+10x}"), -5, TC("D    -5    "), },
		{ TC("D{:<+10x}"), -5, TC("D-5        "), },
		{ TC("D{:=+10x}"), -5, TC("D-        5"), },

		{ TC("D{:>-10x}"), -5, TC("D        -5"), },
		{ TC("D{:^-10x}"), -5, TC("D    -5    "), },
		{ TC("D{:<-10x}"), -5, TC("D-5        "), },
		{ TC("D{:=-10x}"), -5, TC("D-        5"), },

		{ TC("E{:>10}"),  5, TC("E         5"), },
		{ TC("E{:^10}"),  5, TC("E    5     "), },
		{ TC("E{:<10}"),  5, TC("E5         "), },
		{ TC("E{:=10}"),  5, TC("E         5"), },

		{ TC("E{:>+10}"), 5, TC("E        +5"), },
		{ TC("E{:^+10}"), 5, TC("E    +5    "), },
		{ TC("E{:<+10}"), 5, TC("E+5        "), },
		{ TC("E{:=+10}"), 5, TC("E+        5"), },

		{ TC("E{:>-10}"), 5, TC("E         5"), },
		{ TC("E{:^-10}"), 5, TC("E    5     "), },
		{ TC("E{:<-10}"), 5, TC("E5         "), },
		{ TC("E{:=-10}"), 5, TC("E         5"), },

		{ TC("E{:> 10}"), 5, TC("E         5"), },
		{ TC("E{:^ 10}"), 5, TC("E     5    "), },
		{ TC("E{:< 10}"), 5, TC("E 5        "), },
		{ TC("E{:= 10}"), 5, TC("E         5"), },

		{ TC("F{:>#10x}"), -5, TC("F      -0x5"), },
		{ TC("F{:^#10x}"), -5, TC("F   -0x5   "), },
		{ TC("F{:<#10x}"), -5, TC("F-0x5      "), },
		{ TC("F{:=#10x}"), -5, TC("F-0x      5"), },

		{ TC("F{:>+#10x}"), -5, TC("F      -0x5"), },
		{ TC("F{:^+#10x}"), -5, TC("F   -0x5   "), },
		{ TC("F{:<+#10x}"), -5, TC("F-0x5      "), },
		{ TC("F{:=+#10x}"), -5, TC("F-0x      5"), },

		{ TC("F{:>-#10x}"), -5, TC("F      -0x5"), },
		{ TC("F{:^-#10x}"), -5, TC("F   -0x5   "), },
		{ TC("F{:<-#10x}"), -5, TC("F-0x5      "), },
		{ TC("F{:=-#10x}"), -5, TC("F-0x      5"), },

		{ TC("F{:> #10x}"), -5, TC("F      -0x5"), },
		{ TC("F{:^ #10x}"), -5, TC("F   -0x5   "), },
		{ TC("F{:< #10x}"), -5, TC("F-0x5      "), },
		{ TC("F{:= #10x}"), -5, TC("F-0x      5"), },

		{ TC("G{:>#10x}"), 5, TC("G       0x5"), },
		{ TC("G{:^#10x}"), 5, TC("G   0x5    "), },
		{ TC("G{:<#10x}"), 5, TC("G0x5       "), },
		{ TC("G{:=#10x}"), 5, TC("G0x       5"), },

		{ TC("G{:>+#10x}"), 5, TC("G      +0x5"), },
		{ TC("G{:^+#10x}"), 5, TC("G   +0x5   "), },
		{ TC("G{:<+#10x}"), 5, TC("G+0x5      "), },
		{ TC("G{:=+#10x}"), 5, TC("G+0x      5"), },

		{ TC("G{:>-#10x}"), 5, TC("G       0x5"), },
		{ TC("G{:^-#10x}"), 5, TC("G   0x5    "), },
		{ TC("G{:<-#10x}"), 5, TC("G0x5       "), },
		{ TC("G{:=-#10x}"), 5, TC("G0x       5"), },

		{ TC("G{:> #10x}"), 5, TC("G       0x5"), },
		{ TC("G{:^ #10x}"), 5, TC("G    0x5   "), },
		{ TC("G{:< #10x}"), 5, TC("G 0x5      "), },
		{ TC("G{:= #10x}"), 5, TC("G 0x      5"), },

		{ TC("H{:@> #10x}"), 5, TC("H@@@@@@ 0x5"), },
		{ TC("H{:@^ #10x}"), 5, TC("H@@@ 0x5@@@"), },
		{ TC("H{:@< #10x}"), 5, TC("H 0x5@@@@@@"), },
		{ TC("H{:@= #10x}"), 5, TC("H 0x@@@@@@5"), },

		{ TC("H{:\x02> #10x}"), 5, TC("H\x02\x02\x02\x02\x02\x02 0x5"), },
		{ TC("H{:\x02^ #10x}"), 5, TC("H\x02\x02\x02 0x5\x02\x02\x02"), },
		{ TC("H{:\x02< #10x}"), 5, TC("H 0x5\x02\x02\x02\x02\x02\x02"), },
		{ TC("H{:\x02= #10x}"), 5, TC("H 0x\x02\x02\x02\x02\x02\x02""5"), },
		{ TC("H{:@> #10x}"), 5, TC("H@@@@@@ 0x5"), },
		{ TC("H{:@^ #10x}"), 5, TC("H@@@ 0x5@@@"), },
		{ TC("H{:@< #10x}"), 5, TC("H 0x5@@@@@@"), },
		{ TC("H{:@= #10x}"), 5, TC("H 0x@@@@@@5"), },

		{ TC("H{:\x02> #10x}"), 5, TC("H\x02\x02\x02\x02\x02\x02 0x5"), },
		{ TC("H{:\x02^ #10x}"), 5, TC("H\x02\x02\x02 0x5\x02\x02\x02"), },
		{ TC("H{:\x02< #10x}"), 5, TC("H 0x5\x02\x02\x02\x02\x02\x02"), },
		{ TC("H{:\x02= #10x}"), 5, TC("H 0x\x02\x02\x02\x02\x02\x02""5"), },

		{ TC("I{:@> #10x}"), 254, TC("I@@@@@ 0xfe"), },
		{ TC("I{:@^ #10x}"), 254, TC("I@@ 0xfe@@@"), },
		{ TC("I{:@< #10x}"), 254, TC("I 0xfe@@@@@"), },
		{ TC("I{:@= #10x}"), 254, TC("I 0x@@@@@fe"), },

		{ TC("I{:@> #10X}"), 254, TC("I@@@@@ 0XFE"), },
		{ TC("I{:@^ #10X}"), 254, TC("I@@ 0XFE@@@"), },
		{ TC("I{:@< #10X}"), 254, TC("I 0XFE@@@@@"), },
		{ TC("I{:@= #10X}"), 254, TC("I 0X@@@@@FE"), },

};

#define _yIO_inttest_DECL(suffix, type) \
\
static inline \
int _yIO_inttest_ ## suffix(void) { \
	int err = 0; \
	TCHAR buf[256]; \
	for (size_t i = 0; i < sizeof(_yIO_inttests)/sizeof(_yIO_inttests[0]); ++i) { \
 \
	 	 if ((type)-1 > 0 && _yIO_inttests[i].arg < 0) continue; \
						\
		memset(buf, 0, sizeof(buf)); \
		if (1) { \
			TFPRINTF(stderr, "Testing yprintf(\"%"TPRI"\", %d) -> %d\"%"TPRI"\"\n", \
					_yIO_inttests[i].fmt, _yIO_inttests[i].arg, \
					(int)TSTRLEN(_yIO_inttests[i].output), _yIO_inttests[i].output); \
		} \
 \
		const int ret = yπsprintf(buf, sizeof(buf), _yIO_inttests[i].fmt, (type)_yIO_inttests[i].arg); \
 \
		if (ret < 0) { \
			TFPRINTF(stderr, "%s:%d: %d: printf(\"%"TPRI"\", %d) -> %d\n", \
					__FILE__, __LINE__, (int)i, \
					_yIO_inttests[i].fmt, _yIO_inttests[i].arg, ret); \
			err = i + 1; \
		} \
		if (TSTRCMP(buf, _yIO_inttests[i].output) != 0) { \
			TFPRINTF(stderr, "%s:%d: %d: printf(\"%"TPRI"\", %d) -> %d\"%"TPRI"\" should be %d\"%"TPRI"\"\n", \
					__FILE__, __LINE__, (int)i, \
					_yIO_inttests[i].fmt, _yIO_inttests[i].arg, \
					(int)TSTRLEN(buf), buf, \
					(int)TSTRLEN(_yIO_inttests[i].output), _yIO_inttests[i].output); \
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
