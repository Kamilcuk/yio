/**
 * @file
 * @date 2020-lip-31
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "yio_strconv.h"

#ifdef _yIO_HAS_UCHAR_H
int _yIO_conv_c32s_to_mbs(const char32_t *c32s, size_t c32s_len, char **out, size_t *out_len) {
	char *mb = malloc(MB_CUR_MAX);
	if (mb == NULL) return YIO_ERROR_ENOMEM;

	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));

	char *assign = mb;
	size_t i_len = c32s_len;
	for (const char32_t *i = c32s; i_len--; ++i) {
		size_t r = c32rtomb(assign, *i, &ps);
		if (r == (size_t)-1) {
			free(mb);
			return YIO_ERROR_c32rtomb;
		}
		assign += r;
	}
	const size_t mb_len = assign - mb;

	*out = mb;
	*out_len = mb_len;
	return 0;
}

int _yIO_conv_c32s_to_wcs(const char32_t *c32s, size_t c32s_len, wchar_t **out, size_t *out_len) {
#if __STDC_UTF_32__ && __STDC_ISO_10646__
	*out = malloc(c32s_len * sizeof(**out));
	if (*out == NULL) {
		free(*out);
		return YIO_ERROR_ENOMEM;
	}
	for (size_t i = 0; i < c32s_len; ++i) {
		(*out)[i] = c32s[i];
	}
	*out_len = c32s_len;
	return 0;
#else
	// TODO: check if wchar == UTF-32
	int ret = 0;

	char *mb;
	size_t mb_len;
	ret = _yIO_conv_c32s_to_mbs(c32s, c32s_len, &mb, &mb_len);
	if (ret) return ret;

	mbstate_t ps; memset(&ps, 0, sizeof(ps));
	wchar_t *wcs = malloc(MB_CUR_MAX);
	if (wcs == NULL) {
		free(mb);
		return YIO_ERROR_ENOMEM:
	}
	const char *pmb = mb;
	size_t r = mbsrtowcs(wcs, &pmb, mb_len, &ps);
	free(mb);
	if (r == (size_t)-1 || r == (size_t)-2) {
		return YIO_ERROR_MBTOWC_ERR;
	}

	*out = wcs;
	*out_len = r;
	return 0;
#endif
}
#endif
