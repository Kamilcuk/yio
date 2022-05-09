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
	_yIO_TEST(yπprintf(TC("text01 {"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text02 {:"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text03 { :}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text04 {:<"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text05 {:<"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text06 {:1<"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text07 {:<'}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text08 {:''}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text09 {'}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text10 {f}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text11 {0#}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text12 {:0#}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text13 {:0<+#0abc}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text14 {:0<'}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text15 {:0<+'}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text16 {:0<+#'}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text17 {:0<+#0'}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text18 {:0<+#0_'}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text19 {:0<+#0123_'}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text20 {:0<+#0123_.}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text21 {:0<+#0123_.123'}"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text22 {:0"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text23 {:0<"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text24 {:0<  "), 1) != 0);
	_yIO_TEST(yπprintf(TC("text25 {:0<+"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text26 {:0<+  "), 1) != 0);
	_yIO_TEST(yπprintf(TC("text27 {:0<+#"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text28 {:0<+#  "), 1) != 0);
	_yIO_TEST(yπprintf(TC("text29 {:0<+#0"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text30 {:0<+#0  "), 1) != 0);
	_yIO_TEST(yπprintf(TC("text31 {:0<+#0_  "), 1) != 0);
	_yIO_TEST(yπprintf(TC("text32 {:0<+#0123_"), 1) != 0);
	_yIO_TEST(yπprintf(TC("text33 {:0<+#0123_  "), 1) != 0);
	_yIO_TEST(yπprintf(TC("text34 {:0<+#0123_. "), 1) != 0);
	_yIO_TEST(yπprintf(TC("text35 {:0<+#0123_."), 1) != 0);
	_yIO_TEST(yπprintf(TC("text36 {:0<+#0123_.123 "), 1) != 0);
	_yIO_TEST(yπprintf(TC("text37 {:0<+#0123_.123"), 1) != 0);
}
