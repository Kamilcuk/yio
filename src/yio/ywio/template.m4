m4_divert(-1)
/**
 * @file
 * @author Kamil Cukrowski
 * @date 25.05.2020
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 **/
m4_define_name(`_yIO_TYPE_YWIO~, `1~)
m4_define_name(`Ychar~, `wchar_t~)
m4_define_name(`Yint~, `wint_t~)
m4_define_name(`YEOF~, `WEOF~)
m4_define_function(`Yc~, `L$@~)

m4_define_function(`Yisprint~, `iswprint((unsigned char)$@)~)
m4_define_function(`Yisupper~, `iswupper((unsigned char)$@)~)
m4_define_function(`Yisdigit~, `iswdigit((unsigned char)$@)~)

m4_define_function(`Ystrlen~, `wcslen($@)~)
m4_define_function(`Ystrchr~, `wcschr($@)~)
m4_define_function(`Ystrcmp~, `wcscmp($@)~)

m4_define_function(`Yfprintf~, `fprintf($@)~)
m4_define_function(`Yprintf~, `printf($@)~)
m4_define_name(`YPRI~, `"ls"~)

m4_divert(0)m4_dnl
