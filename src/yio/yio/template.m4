m4_divert(-1)
/**
 * @file
 * @author Kamil Cukrowski
 * @date 25.05.2020
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 **/
m4_define_name(`_yIO_TYPE_YIO~, `1~)
m4_define_name(`Ychar~, `char~)
m4_define_name(`Yint~, `int~)
m4_define_name(`YEOF~, `EOF~)

m4_define_function(`Yc~, `$@~)
m4_define_function(`Yisprint~, `isprint((unsigned char)$@)~)
m4_define_function(`Yisupper~, `isupper((unsigned char)$@)~)
m4_define_function(`Yisdigit~, `isdigit((unsigned char)$@)~)
m4_define_function(`Ystrlen~, `strlen($@)~)
m4_define_function(`Yfgetc~, `fgetc($@)~)
m4_define_function(`Ystrchr~, `strchr($@)~)
m4_define_function(`Ystrcmp~, `strcmp($@)~)

m4_define_function(`Yfprintf~, `fprintf($@)~)
m4_define_function(`Yprintf~, `printf($@)~)
m4_define(`YPRI~, `"s"~)

m4_divert(0)m4_dnl
