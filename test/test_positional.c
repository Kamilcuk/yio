#include <yio_test.h>
int main() {
	YIO_TEST("3 2 1", "{2} {1} {0}", 1, 2, 3);
	YIO_TEST(
			"18 17 16 0xf 0xe 13 12 11 10 9 8 7 6 5 4 3 2 1 0",
			"{18:%S} {17} {16} {15} {14} {13:.0f} {12:.0f} {11:.0f} {10} {9} {8} {7} {6} {5} {4} {3} {2} {1} {0}",
			(char)'0',
			(unsigned char)'1',
			(signed char)'2',
			(short)3,
			(unsigned short)4,
			5, 6u, 7l, 8lu, 9ll, 10llu,
			11.0f, 12.0, 13.0L,
			(void*)(uintptr_t)14,
			(const void *)(uintptr_t)15,
			"16",
			L"17",
			(struct tm){.tm_sec=18} );
	YIO_TEST("a, b, c", "{0}, {1}, {2}", "a", "b", "c");
	YIO_TEST("a, b, c", "{}, {}, {}", "a", "b", "c");
	YIO_TEST("c, b, a", "{2}, {1}, {0}", "a", "b", "c");
	YIO_TEST("abracadabra", "{0}{1}{0}", "abra", "cad");
	YIO_TEST("left<<<<<<<<<<<<", "{0:<<{1}}", "left", 16);
	YIO_TEST("str___ ___abc", "{1:_<{0}.{2}} {3:_>{0}.{2}}", 6, "str", 3, "abc");
}
