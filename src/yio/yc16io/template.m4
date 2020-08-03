m4_divert(-1)
/**
 * @file
 * @author Kamil Cukrowski
 * @date 25.05.2020
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 **/
m4_define_name(`_yIO_TYPE_YC16IO~, `1~)
m4_define_name(`Ychar~, `uint16_t~)
m4_define_name(`Yint~, `uint16_t~)
m4_define_name(`YEOF~, `UINT16_MAX~)

m4_define_function(`Yc~, `u$@~)
m4_define_function(`Yisprint~, `uc_is_print((unsigned char)$@)~)
m4_define_function(`Yisupper~, `uc_is_upper((unsigned char)$@)~)
m4_define_function(`Yisdigit~, `uc_is_digit((unsigned char)$@)~)
m4_define_function(`Ystrlen~, `u16_strlen($@)~)
m4_define_function(`Ystrchr~, `u16_strchr($@)~)
m4_define_function(`Ystrcmp~, `u16_strcmp($@)~)

m4_define_function(`Yfprintf~, `ulc_fprintf($@)~)
m4_define_function(`Yprintf~, `ulc_fprintf(stdout, $@)~)
m4_define_name(`YPRI~, `"lU"~)

m4_divert(0)m4_dnl
