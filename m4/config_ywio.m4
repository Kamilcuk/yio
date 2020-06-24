m4_divert(-1)
/**
 * @file
 * @author Kamil Cukrowski
 * @date 25.05.2020
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_define(`m4_lib_header_suffix~, `m4_dnl
#include <wctype.h>
#include <wchar.h>
m4_dnl ~)

m4_define_name(`YTYPE~, `YTYPE_YWIO~)

m4_define_name(`Ychar~, `wchar_t~)
m4_define_name(`Yint~, `wint_t~)
m4_define_name(`YEOF~, `WEOF~)
m4_define_function(`Yc~, `L$1~)
m4_define_function(`Yisprint~, `iswprint~)
m4_define_function(`Ystrlen~, `wcslen~)
m4_define_function(`Yfgetc~, `fgetwc~)

m4_divert(0)m4_dnl
