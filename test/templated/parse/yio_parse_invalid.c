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
	YΩIO_TEST_FAIL("text01 {", 1);
	YΩIO_TEST_FAIL("text02 {:", 1);
	YΩIO_TEST_FAIL("text03 { :}", 1);
	YΩIO_TEST_FAIL("text04 {:<", 1);
	YΩIO_TEST_FAIL("text05 {:<", 1);
	YΩIO_TEST_FAIL("text06 {:1<", 1);
	YΩIO_TEST_FAIL("text07 {:<'}", 1);
	YΩIO_TEST_FAIL("text08 {:''}", 1);
	YΩIO_TEST_FAIL("text09 {'}", 1);
	YΩIO_TEST_FAIL("text10 {f}", 1);
	YΩIO_TEST_FAIL("text11 {0#}", 1);
	// YΩIO_TEST_FAIL("text12 {:0#}", 1);
	YΩIO_TEST_FAIL("text13 {:0<+#0abc}", 1);
	YΩIO_TEST_FAIL("text14 {:0<'}", 1);
	YΩIO_TEST_FAIL("text15 {:0<+'}", 1);
	YΩIO_TEST_FAIL("text16 {:0<+#'}", 1);
	YΩIO_TEST_FAIL("text17 {:0<+#0'}", 1);
	YΩIO_TEST_FAIL("text18 {:0<+#0_'}", 1);
	YΩIO_TEST_FAIL("text19 {:0<+#0123_'}", 1);
	YΩIO_TEST_FAIL("text20 {:0<+#0123_.}", 1);
	YΩIO_TEST_FAIL("text21 {:0<+#0123_.123'}", 1);
	YΩIO_TEST_FAIL("text22 {:0", 1);
	YΩIO_TEST_FAIL("text23 {:0<", 1);
	YΩIO_TEST_FAIL("text24 {:0<  ", 1);
	YΩIO_TEST_FAIL("text25 {:0<+", 1);
	YΩIO_TEST_FAIL("text26 {:0<+  ", 1);
	YΩIO_TEST_FAIL("text27 {:0<+#", 1);
	YΩIO_TEST_FAIL("text28 {:0<+#  ", 1);
	YΩIO_TEST_FAIL("text29 {:0<+#0", 1);
	YΩIO_TEST_FAIL("text30 {:0<+#0  ", 1);
	YΩIO_TEST_FAIL("text31 {:0<+#0_  ", 1);
	YΩIO_TEST_FAIL("text32 {:0<+#0123_", 1);
	YΩIO_TEST_FAIL("text33 {:0<+#0123_  ", 1);
	YΩIO_TEST_FAIL("text34 {:0<+#0123_. ", 1);
	YΩIO_TEST_FAIL("text35 {:0<+#0123_.", 1);
	YΩIO_TEST_FAIL("text36 {:0<+#0123_.123 ", 1);
	YΩIO_TEST_FAIL("text37 {:0<+#0123_.123", 1);
}
