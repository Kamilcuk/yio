#include <yio_test.h>
int main() {
	YIO_TEST("3 2 1", "{2} {1} {0}", 1, 2, 3);
	YIO_TEST(
			"17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0",
			"{17:%S} {16} {15} {14} {13} {12:.0} {11:.0} {10:.0} {9} {8} {7} {6} {5} {4} {3} {2} {1} {0}",
			(char)'0',
			(unsigned char)'1',
			(signed char)'2',
			(short)3,
			(unsigned short)4,
			4, 5u, 6l, 7lu, 8ll, 9llu,
			10.0f, 11.0, 12.0L,
			(void*)(uintptr_t)13,
			(const void *)(uintptr_t)14,
			"15",
			L"16",
			(struct tm){.tm_sec=17} );

}
