/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#ifdef _yIO_HAS_UCHAR_H
#include <uchar.h>

#ifdef _yIO_HAS_UNISTRING
#include <unistr.h>
#endif

static inline
int _yΩIO_print_constornot_char32pnt(yπio_printctx_t *t, const char32_t *str) {
	const int err = yπio_printctx_init(t);
	if (err) return err;
	const struct yπio_printfmt_s *pf = yπio_printctx_get_fmt(t);
	const size_t len = _yIO_c32nlen(str, pf->precision >= 0 ? (size_t)pf->precision : SIZE_MAX);
m4_template_chooser(`m4_dnl);
	int ret = 0;
	char *mb;
	size_t mb_len;
	ret = _yIO_conv_c32s_to_mbs(str, len, &mb, &mb_len);
	if (ret) return ret;
	ret = yπio_printctx_put(t, mb, mb_len);
	free(mb);
	return ret;
~, `m4_dnl;
	int ret = 0;
	wchar_t *wc;
	size_t wc_len;
	ret = _yIO_conv_c32s_to_wcs(str, len, &wc, &wc_len);
	if (ret) return ret;
	ret = yπio_printctx_putπ(t, wc, wc_len);
	free(wc);
	return ret;
~, `m4_dnl;
	return yπio_printctx_putπ(t, str, len);
~)m4_dnl;
}

/* ------------------------------------------------------ */

int _yΩIO_print_char32pnt(yπio_printctx_t *t) {
	return _yΩIO_print_constornot_char32pnt(t, yπio_printctx_va_arg(t, char32_t *));
}

int _yΩIO_print_constchar32pnt(yπio_printctx_t *t) {
	return _yΩIO_print_constornot_char32pnt(t, yπio_printctx_va_arg(t, const char32_t *));
}

#endif
