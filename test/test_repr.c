#include <yio_test.h>

int main() {
	YIO_TEST(( .eq="a\\1\\2b", .ret=6 ), "{}", yio_repr_oct("a\001\2b"));
	YIO_TEST("a\\1+", "{}", yio_repr_oct("a\x01\x02b"));
	YIO_TEST("a\\0011", "{}", yio_repr_oct("a\0011"));
	YIO_TEST("?\\?=", "{}", yio_repr_oct("?\?="));
}
