/*
 * yio_parse_invalid.c
 *
 *  Created on: 4 kwi 2020
 *      Author: kamil
 */
#include <yio_test.h>
#include <yio_test_private.h>
#include <yπio.h>
int main() {
	// {:[[fill]><=^][+- ][#][0][width][_,][.precision][bcdeEfFgGnosxX%]}
	_yIO_TEST(yπprintf(Yc("text {"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text { :}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:<"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:<"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:1<"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:<'}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:''}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {'}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {f}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {0#}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0#}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0abc}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<'}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+'}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#'}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0'}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0_'}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0123_'}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0123_.}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0123_.123'}"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<  "), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+  "), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#  "), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0  "), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0_  "), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0123_"), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0123_  "), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0123_. "), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0123_."), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0123_.123 "), 1) != 0);
	_yIO_TEST(yπprintf(Yc("text {:0<+#0123_.123"), 1) != 0);
}
