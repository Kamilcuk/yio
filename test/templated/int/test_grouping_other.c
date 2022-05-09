#include <yio_test.h>
#include <yπio.h>
int main() {
	YΩIO_TEST("0b1111_0000_1111_0000", "{:#_b}", 0xf0f0);
	YΩIO_TEST("0x1122_3344", "{:#_x}", 0x11223344);
	YΩIO_TEST("11_223_344", "{:_d}", 11223344);
}
