m4_divert(-1)
/**
 * @file cfg_yio.m4
 * @author Kamil Cukrowski
 * @date 25.05.2020
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 **/
m4_define_name(`m4_lib_header_suffix~, `~)
m4_define_name(`YTYPE~, `YTYPE_YIO~)
m4_define_name(`Y_IO~, `_yIO_$@~)
m4_define_name(`YIO~, `YIO_$@~)
m4_define_name(`Yio~, `yio_$@~)
m4_define_name(`Ychar~, `char~)
m4_define_name(`Yint~, `int~)
m4_define_name(`YEOF~, `EOF~)
m4_define_func(`Yc~, `$@~)
m4_define_func(`Yisprint~, `isprint((unsigned char)$@)~)
m4_define_func(`Yisupper~, `isupper((unsigned char)$@)~)
m4_define_func(`Yisdigit~, `isdigit((unsigned char)$@)~)
m4_define_func(`Ystrlen~, `strlen($@)~)
m4_define_func(`Yfgetc~, `fgetc($@)~)
m4_define_func(`Ystrchr~, `strchr($@)~)

m4_divert(0)m4_dnl