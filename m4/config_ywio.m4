m4_divert(-1)
/**
 * @file
 * @author Kamil Cukrowski
 * @date 25.05.2020
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_define(`YHEADERS~, `m4_dnl
#include <wctype.h>
#include <wchar.h>
m4_dnl ~)

m4_define_name(`YOMEGA~, `w~)
m4_define_name(`YPI~, `w~)

#define YTYPE_YIO  0
m4_define_name(`YTYPE_YIO~,  `0~)
#define YTYPE_YWIO  1
m4_define_name(`YTYPE_YWIO~, `1~)
#define YTYPE_YUIO  0
m4_define_name(`YTYPE_YUIO~, `0~)

#define Ychar wchar_t
m4_define_name(`Ychar~, `wchar_t~)
#define Yint wint_t
m4_define_name(`Yint~, `wint_t~)
#define YEOF WEOF
m4_define_name(`YEOF~, `WEOF~)
#define Yc(x)  L##x
m4_define_function(`Yc~, `L$1~)
#define Yisprint(x)  iswprint(x)
m4_define_function(`Yisprint~, `iswprint~)
#define Ystrlen(x)  wcslen(x)
m4_define_function(`Ystrlen~, `wcslen~)
#define Yfgetc(x)  fgetwc(x)
m4_define_function(`Yfgetc~, `fgetwc~)

m4_divert(0)m4_dnl
