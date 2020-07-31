m4_divert(-1)
/**
 * @file
 * @author Kamil Cukrowski
 * @date 25.05.2020
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 **/
m4_define_name(`_yIO_TYPE_YUIO~, `1~)
m4_define_name(`Ychar~, `uint32_t~)
m4_define_name(`Yint~, `uint32_t~)
m4_define_name(`YEOF~, `UINT32_C(0xFFFFFFFF)~)

m4_define_function(`Yc~, `U$@~)
m4_define_function(`Yisprint~, `uc_is_print((unsigned char)$@)~)
m4_define_function(`Yisupper~, `uc_is_upper((unsigned char)$@)~)
m4_define_function(`Yisdigit~, `uc_is_digit((unsigned char)$@)~)
m4_define_function(`Ystrlen~, `u32_strlen($@)~)
m4_define_function(`Yfgetc~, `fgetc($@)~)
m4_define_function(`Ystrchr~, `u32_strchr($@)~)
m4_define_function(`Ystrcmp~, `u32_cmp($@)~)

m4_define_function(`Yfprintf~, `ulc_fprintf($@)~)
m4_define_function(`Yprintf~, `ulc_printf($@)~)
m4_define_name(`YPRI~, `"llU"~)

m4_divert(0)m4_dnl
