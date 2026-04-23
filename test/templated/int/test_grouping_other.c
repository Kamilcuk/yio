#include <yio_test.h>
#include <yio.h>
int main() {
	YIO_TEST("0b1111_0000_1111_0000", "{:#_b}", 0xf0f0);
	YIO_TEST("0x1122_3344", "{:#_x}", 0x11223344);
	YIO_TEST("11_223_344", "{:_d}", 11223344);
}
