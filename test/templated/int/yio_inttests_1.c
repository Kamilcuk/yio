/*
 * yio_unittest.c
 *
 *  Created on: 19 pa� 2019
 *      Author: Kamil
 */
#include <yio_test.h>
#include <string.h>

struct YIO_inttests_s {
	const char *fmt;
	int arg;
	const char *output;
};

static const struct YIO_inttests_s YIO_inttests[] = {
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
		{ "B{:^10d}",  5, "B    5     ", },
		{ "B{:<10d}",  5, "B5         ", },
		{ "B{:=10d}",  5, "B         5", },

		{ "B{:>+10d}", 5, "B        +5", },
		{ "B{:^+10d}", 5, "B    +5    ", },
		{ "B{:<+10d}", 5, "B+5        ", },
		{ "B{:=+10d}", 5, "B+        5", },

		{ "B{:>-10d}", 5, "B         5", },
		{ "B{:^-10d}", 5, "B    5     ", },
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
		{ "E{:^10}",  5, "E    5     ", },
		{ "E{:<10}",  5, "E5         ", },
		{ "E{:=10}",  5, "E         5", },

		{ "E{:>+10}", 5, "E        +5", },
		{ "E{:^+10}", 5, "E    +5    ", },
		{ "E{:<+10}", 5, "E+5        ", },
		{ "E{:=+10}", 5, "E+        5", },

		{ "E{:>-10}", 5, "E         5", },
		{ "E{:^-10}", 5, "E    5     ", },
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
		{ "G{:^#10x}", 5, "G   0x5    ", },
		{ "G{:<#10x}", 5, "G0x5       ", },
		{ "G{:=#10x}", 5, "G0x       5", },

		{ "G{:>+#10x}", 5, "G      +0x5", },
		{ "G{:^+#10x}", 5, "G   +0x5   ", },
		{ "G{:<+#10x}", 5, "G+0x5      ", },
		{ "G{:=+#10x}", 5, "G+0x      5", },

		{ "G{:>-#10x}", 5, "G       0x5", },
		{ "G{:^-#10x}", 5, "G   0x5    ", },
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
		{ "H{:\x02= #10x}", 5, "H 0x\x02\x02\x02\x02\x02\x02""5", },
		{ "H{:@> #10x}", 5, "H@@@@@@ 0x5", },
		{ "H{:@^ #10x}", 5, "H@@@ 0x5@@@", },
		{ "H{:@< #10x}", 5, "H 0x5@@@@@@", },
		{ "H{:@= #10x}", 5, "H 0x@@@@@@5", },

		{ "H{:\x02> #10x}", 5, "H\x02\x02\x02\x02\x02\x02 0x5", },
		{ "H{:\x02^ #10x}", 5, "H\x02\x02\x02 0x5\x02\x02\x02", },
		{ "H{:\x02< #10x}", 5, "H 0x5\x02\x02\x02\x02\x02\x02", },
		{ "H{:\x02= #10x}", 5, "H 0x\x02\x02\x02\x02\x02\x02""5", },

		{ "I{:@> #10x}", 254, "I@@@@@ 0xfe", },
		{ "I{:@^ #10x}", 254, "I@@ 0xfe@@@", },
		{ "I{:@< #10x}", 254, "I 0xfe@@@@@", },
		{ "I{:@= #10x}", 254, "I 0x@@@@@fe", },

		{ "I{:@> #10X}", 254, "I@@@@@ 0XFE", },
		{ "I{:@^ #10X}", 254, "I@@ 0XFE@@@", },
		{ "I{:@< #10X}", 254, "I 0XFE@@@@@", },
		{ "I{:@= #10X}", 254, "I 0X@@@@@FE", },

};

#define YIO_inttest_DECL(suffix, type) \
\
static inline \
int YIO_inttest_ ## suffix(void) { \
	int err = 0; \
	char buf[256]; \
	for (size_t i = 0; i < sizeof(YIO_inttests)/sizeof(YIO_inttests[0]); ++i) { \
 \
	 	 if ((type)-1 > 0 && YIO_inttests[i].arg < 0) continue; \
						\
		memset(buf, 0, sizeof(buf)); \
		if (1) { \
			fprintf(stderr, "Testing yio_print(\"%""s""\", %d) -> %d\"%""s""\"\n", \
					YIO_inttests[i].fmt, YIO_inttests[i].arg, \
					(int)strlen(YIO_inttests[i].output), YIO_inttests[i].output); \
		} \
 \
		const int ret = yio_snprint(buf, sizeof(buf), YIO_inttests[i].fmt, (type)YIO_inttests[i].arg); \
 \
		if (ret < 0) { \
			fprintf(stderr, "%s:%d: %d: printf(\"%""s""\", %d) -> %d\n", \
					__FILE__, __LINE__, (int)i, \
					YIO_inttests[i].fmt, YIO_inttests[i].arg, ret); \
			err = i + 1; \
		} \
		if (strcmp(buf, YIO_inttests[i].output) != 0) { \
			fprintf(stderr, "%s:%d: %d: printf(\"%""s""\", %d) -> %d\"%""s""\" should be %d\"%""s""\"\n", \
					__FILE__, __LINE__, (int)i, \
					YIO_inttests[i].fmt, YIO_inttests[i].arg, \
					(int)strlen(buf), buf, \
					(int)strlen(YIO_inttests[i].output), YIO_inttests[i].output); \
			err = i + 1; \
		} \
 \
	} \
	return err; \
}

YIO_inttest_DECL(short, short)
YIO_inttest_DECL(ushort, unsigned short)
YIO_inttest_DECL(int, int)
YIO_inttest_DECL(uint, unsigned int)
YIO_inttest_DECL(long, long)
YIO_inttest_DECL(ulong, unsigned long)
YIO_inttest_DECL(llong, long long)
YIO_inttest_DECL(ullong, unsigned long long)

#define YIO_inttest_DO(suffix) \
		err = YIO_inttest_ ## suffix(); \
		if (err) return err;

int main() {
	int err = 0;
	YIO_inttest_DO(int);
	YIO_inttest_DO(uint);
	YIO_inttest_DO(short);
	YIO_inttest_DO(ushort);
	YIO_inttest_DO(long);
	YIO_inttest_DO(ulong);
	YIO_inttest_DO(llong);
	YIO_inttest_DO(ullong);
	return 0;
}
