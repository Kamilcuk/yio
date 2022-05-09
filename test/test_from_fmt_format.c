#include <yio_test.h>
int main() {
	YIO_TEST("I'd rather be happy than right.", "I'd rather be {1} than {0}.", "right", "happy");
	YIO_TEST_FAIL("The answer is {:d}", "forty-two");
	YIO_TEST("abracadabra", "{0}{1}{0}", "abra", "cad");
	YIO_TEST("\\+3.140000; -3.140000", "{:+f}; {:+f}", 3.14, -3.14);
	YIO_TEST(" 3.140000; -3.140000","{: f}; {: f}", 3.14, -3.14);
	YIO_TEST("3.140000; -3.140000","{:-f}; {:-f}", 3.14, -3.14);
	YIO_TEST("3.1", "{:.{}f}", 3.14, 1);
	YIO_TEST("int: 42;  hex: 2a;  oct: 52; bin: 101010", "int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
	YIO_TEST("int: 42;  hex: 0x2a;  oct: 0o52;  bin: 0b101010", "int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42);
	YIO_TEST("0x00", "{:#04x}", 0);
	YIO_TEST(
		"|--------------------|\n"
		"|   Hello, world!    |\n"
		"|--------------------|\n"
		,
		"|{0:-^{2}}|\n"
		"|{1: ^{2}}|\n"
		"|{0:-^{2}}|\n", "", "Hello, world!", 20);
}
