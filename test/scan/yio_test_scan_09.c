/**
 * @file yio_test_scan_09.c
 * @date 2020-05-14
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio.h>
#include <yio_test_private.h>
#define _yIO_dbgvarln(v)  yprint(#v " = ", v, "\n")
int main() {
    char a[5];
    char b[20] = {0};
    char c[20] = {0};
    int d = 10;
    int err = ysscanf("1234567890 def geh", "{}{} {} {}", &a, &d, &b, &c).error;

    _yIO_dbgvarln(err);
    _yIO_dbgvarln(a);
    _yIO_dbgvarln(b);
    _yIO_dbgvarln(c);
    _yIO_dbgvarln(d);

    _yIO_TEST(err == 0);
    _yIO_TEST(strcmp(a, "1234") == 0);
    _yIO_TEST(d == 567890);
    _yIO_TEST(strcmp(b, "def") == 0);
    _yIO_TEST(strcmp(c, "geh") == 0);
}
