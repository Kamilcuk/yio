/*
 * yio_unittest.c
 *
 *  Created on: 19 pa� 2019
 *      Author: Kamil
 */
#include <yio.h>
#include <string.h>

struct _yIO_inttests_s {
	const char *fmt;
	int arg;
	const char *output;
};

static const struct _yIO_inttests_s _yIO_inttests[] = {
		{ "A{}", 5, "A5", },
		{ "A{:}", 5, "A5", },

		{ "A{:d}", 5, "A5", },
		{ "A{:x}", 5, "A5", },
		{ "A{:X}", 5, "A5", },

		{ "A{:x}", 0xa, "Aa", },
		{ "A{:X}", 0xa, "AA", },
		{ "A{:o}", 077, "A77", },

		{ "A{:#o}", 077, "A0o77", },
		{ "A{:#x}", 0xa, "A0xa", },
		{ "A{:#X}", 0xA, "A0XA", },

		{ "A{:010d}", 5, "A0000000005", },

		{ "B{:>10d}",  5, "B         5", },
		{ "B{:^10d}",  5, "B     5    ", },
		{ "B{:<10d}",  5, "B5         ", },
		{ "B{:=10d}",  5, "B         5", },

		{ "B{:>+10d}", 5, "B        +5", },
		{ "B{:^+10d}", 5, "B    +5    ", },
		{ "B{:<+10d}", 5, "B+5        ", },
		{ "B{:=+10d}", 5, "B+        5", },

		{ "B{:>-10d}", 5, "B         5", },
		{ "B{:^-10d}", 5, "B     5    ", },
		{ "B{:<-10d}", 5, "B5         ", },
		{ "B{:=-10d}", 5, "B         5", },

		{ "B{:> 10d}", 5, "B         5", },
		{ "B{:^ 10d}", 5, "B     5    ", },
		{ "B{:< 10d}", 5, "B 5        ", },
		{ "B{:= 10d}", 5, "B         5", },

		{ "C{:>10d}", -5, "C        -5", },
		{ "C{:^10d}", -5, "C    -5    ", },
		{ "C{:<10d}", -5, "C-5        ", },
		{ "C{:=10d}", -5, "C-        5", },

		{ "C{:>+10x}", -5, "C        -5", },
		{ "C{:^+10x}", -5, "C    -5    ", },
		{ "C{:<+10x}", -5, "C-5        ", },
		{ "C{:=+10x}", -5, "C-        5", },

		{ "C{:>-10d}", -5, "C        -5", },
		{ "C{:^-10d}", -5, "C    -5    ", },
		{ "C{:<-10d}", -5, "C-5        ", },
		{ "C{:=-10d}", -5, "C-        5", },

		{ "D{:> 10x}", -5, "D        -5", },
		{ "D{:^ 10x}", -5, "D    -5    ", },
		{ "D{:< 10x}", -5, "D-5        ", },
		{ "D{:= 10x}", -5, "D-        5", },

		{ "D{:>+10x}", -5, "D        -5", },
		{ "D{:^+10x}", -5, "D    -5    ", },
		{ "D{:<+10x}", -5, "D-5        ", },
		{ "D{:=+10x}", -5, "D-        5", },

		{ "D{:>-10x}", -5, "D        -5", },
		{ "D{:^-10x}", -5, "D    -5    ", },
		{ "D{:<-10x}", -5, "D-5        ", },
		{ "D{:=-10x}", -5, "D-        5", },

		{ "E{:>10}",  5, "E         5", },
		{ "E{:^10}",  5, "E     5    ", },
		{ "E{:<10}",  5, "E5         ", },
		{ "E{:=10}",  5, "E         5", },

		{ "E{:>+10}", 5, "E        +5", },
		{ "E{:^+10}", 5, "E    +5    ", },
		{ "E{:<+10}", 5, "E+5        ", },
		{ "E{:=+10}", 5, "E+        5", },

		{ "E{:>-10}", 5, "E         5", },
		{ "E{:^-10}", 5, "E     5    ", },
		{ "E{:<-10}", 5, "E5         ", },
		{ "E{:=-10}", 5, "E         5", },

		{ "E{:> 10}", 5, "E         5", },
		{ "E{:^ 10}", 5, "E     5    ", },
		{ "E{:< 10}", 5, "E 5        ", },
		{ "E{:= 10}", 5, "E         5", },

		{ "F{:>#10x}", -5, "F      -0x5", },
		{ "F{:^#10x}", -5, "F   -0x5   ", },
		{ "F{:<#10x}", -5, "F-0x5      ", },
		{ "F{:=#10x}", -5, "F-0x      5", },

		{ "F{:>+#10x}", -5, "F      -0x5", },
		{ "F{:^+#10x}", -5, "F   -0x5   ", },
		{ "F{:<+#10x}", -5, "F-0x5      ", },
		{ "F{:=+#10x}", -5, "F-0x      5", },

		{ "F{:>-#10x}", -5, "F      -0x5", },
		{ "F{:^-#10x}", -5, "F   -0x5   ", },
		{ "F{:<-#10x}", -5, "F-0x5      ", },
		{ "F{:=-#10x}", -5, "F-0x      5", },

		{ "F{:> #10x}", -5, "F      -0x5", },
		{ "F{:^ #10x}", -5, "F   -0x5   ", },
		{ "F{:< #10x}", -5, "F-0x5      ", },
		{ "F{:= #10x}", -5, "F-0x      5", },

		{ "G{:>#10x}", 5, "G       0x5", },
		{ "G{:^#10x}", 5, "G    0x5   ", },
		{ "G{:<#10x}", 5, "G0x5       ", },
		{ "G{:=#10x}", 5, "G0x       5", },

		{ "G{:>+#10x}", 5, "G      +0x5", },
		{ "G{:^+#10x}", 5, "G   +0x5   ", },
		{ "G{:<+#10x}", 5, "G+0x5      ", },
		{ "G{:=+#10x}", 5, "G+0x      5", },

		{ "G{:>-#10x}", 5, "G       0x5", },
		{ "G{:^-#10x}", 5, "G    0x5   ", },
		{ "G{:<-#10x}", 5, "G0x5       ", },
		{ "G{:=-#10x}", 5, "G0x       5", },

		{ "G{:> #10x}", 5, "G       0x5", },
		{ "G{:^ #10x}", 5, "G    0x5   ", },
		{ "G{:< #10x}", 5, "G 0x5      ", },
		{ "G{:= #10x}", 5, "G 0x      5", },

		{ "H{:@> #10x}", 5, "H@@@@@@ 0x5", },
		{ "H{:@^ #10x}", 5, "H@@@ 0x5@@@", },
		{ "H{:@< #10x}", 5, "H 0x5@@@@@@", },
		{ "H{:@= #10x}", 5, "H 0x@@@@@@5", },

		{ "H{:\x02> #10x}", 5, "H\x02\x02\x02\x02\x02\x02 0x5", },
		{ "H{:\x02^ #10x}", 5, "H\x02\x02\x02 0x5\x02\x02\x02", },
		{ "H{:\x02< #10x}", 5, "H 0x5\x02\x02\x02\x02\x02\x02", },
		{ "H{:\x02= #10x}", 5, "H 0x\x02\x02\x02\x02\x02\x02" "5", },

};

#define _yIO_inttest_DECL(suffix, type) \
\
static inline \
int _yIO_inttest_ ## suffix(void) { \
	int err = 0; \
	char buf[256]; \
	for (size_t i = 0; i < sizeof(_yIO_inttests)/sizeof(_yIO_inttests[0]); ++i) { \
 \
	 	 if ((type)-1 > 0 && _yIO_inttests[i].arg < 0) continue; \
 \
		memset(buf, 0, sizeof(buf)); \
		if (1) { \
			fprintf(stderr, "Testing yprintf(\"%s\", %d) -> %d\"%s\"\n", \
					_yIO_inttests[i].fmt, _yIO_inttests[i].arg, \
					(int)strlen(_yIO_inttests[i].output), _yIO_inttests[i].output); \
		} \
 \
		const int ret = ysprintf(buf, sizeof(buf), _yIO_inttests[i].fmt, (type)_yIO_inttests[i].arg); \
 \
		if (ret < 0) { \
			fprintf(stderr, "%s:%d: %d: printf(\"%s\", %d) -> %d\n", \
					__FILE__, __LINE__, (int)i, \
					_yIO_inttests[i].fmt, _yIO_inttests[i].arg, ret); \
			err = i + 1; \
		} \
		if (strcmp(buf, _yIO_inttests[i].output) != 0) { \
			fprintf(stderr, "%s:%d: %d: printf(\"%s\", %d) -> %d\"%s\" should be %d\"%s\"\n", \
					__FILE__, __LINE__, (int)i, \
					_yIO_inttests[i].fmt, _yIO_inttests[i].arg, \
					(int)strlen(buf), buf, \
					(int)strlen(_yIO_inttests[i].output), _yIO_inttests[i].output); \
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
