/**
 * @file
 * @date 2020-lip-31
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */

#ifdef _yIO_HAS_WCHAR_H
#include <wchar.h>
_yIO_wur _yIO_access(__read_only__, 1, 2)
int _yIO_conv_wcs_to_mbs(const wchar_t *wcs, size_t wcs_len, char **mb, size_t *mb_len);
_yIO_wur _yIO_access(__read_only__, 1, 2)
int _yIO_conv_mbs_to_wcs(char *mb, size_t mb_len, const wchar_t **wcs, size_t *wcs_len);
#endif

#ifdef _yIO_HAS_UCHAR_H
#include <uchar.h>
_yIO_wur _yIO_access(__read_only__, 1, 2)
int _yIO_conv_c16s_to_mbs(const char16_t *c16s, size_t c16s_len, char **mb, size_t *mb_len);
_yIO_wur _yIO_access(__read_only__, 1, 2)
int _yIO_conv_c16s_to_wcs(const char16_t *c16s, size_t c16s_len, wchar_t **wcs, size_t *wcs_len);
_yIO_wur _yIO_access(__read_only__, 1, 2)
int _yIO_conv_c32s_to_mbs(const char32_t *c32s, size_t c32s_len, char **mb, size_t *mb_len);
_yIO_wur _yIO_access(__read_only__, 1, 2)
int _yIO_conv_c32s_to_wcs(const char32_t *c32s, size_t c32s_len, wchar_t **wcs, size_t *wcs_len);
#endif
