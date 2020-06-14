m4_divert(-1)
/**
 * @file cfg_ywio.m4
 * @author Kamil Cukrowski
 * @date 25.05.2020
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_define(`m4_lib_header_suffix~, `#include <wctype.h>
#include <wchar.h>~)
m4_define_name(`YTYPE~, `YTYPE_YWIO~)
m4_define_name(`Y_IO~, `_yWIO_$@~)
m4_define_name(`YIO~, `YWIO_$@~)
m4_define_name(`Yio~, `ywio_$@~)
m4_define_name(`Ychar~, `wchar_t~)
m4_define_name(`Yint~, `wint_t~)
m4_define_name(`YEOF~, `WEOF~)
m4_define_name(`Yc~, `L$@~)
m4_define_name(`Yisprint~, `iswprint~)
m4_define_name(`Ystrlen~, `wcslen~)
m4_define_name(`Yfgetc~, `fgetwc~)

m4_define_name(ypright, ywpright)
m4_define_name(ypleft, ywpleft)
m4_define_name(_yIO_print_right, _yWIO_print_right)

m4_divert(0)m4_dnl
