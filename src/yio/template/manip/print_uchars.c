/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#if _yIO_HAS_UCHAR_H

static inline
int _yΩIO_print_constornot_char16pnt(yπio_printctx_t *t, const char16_t *str) {
	int err = yπio_printctx_init(t);
	if (err) return err;
	const struct yπio_printfmt_s *pf = yπio_printctx_get_fmt(t);
	const size_t len = _yIO_c16strnlen(str, pf->precision >= 0 ? (size_t)pf->precision : SIZE_MAX);
	const Ychar *dest;
	size_t dest_len;
	err = _yIO_strconv_c16str_to_πstr(str, len, &dest, &dest_len);
	if (err) return err;
	err = yπio_printctx_putπ(t, dest, dest_len);
	_yIO_strconv_free_c16str_to_πstr(str, dest);
	return err;
}

static inline
int _yΩIO_print_constornot_char32pnt(yπio_printctx_t *t, const char32_t *str) {
	int err = yπio_printctx_init(t);
	if (err) return err;
	const struct yπio_printfmt_s *pf = yπio_printctx_get_fmt(t);
	const size_t len = _yIO_ustrnlen(str, pf->precision >= 0 ? (size_t)pf->precision : SIZE_MAX);
	const Ychar *dest;
	size_t dest_len;
	err = _yIO_strconv_ustr_to_πstr(str, len, &dest, &dest_len);
	if (err) return err;
	err = yπio_printctx_putπ(t, dest, dest_len);
	_yIO_strconv_free_ustr_to_πstr(str, dest);
	return err;
}

/* ------------------------------------------------------ */

int _yΩIO_print_constchar16pnt(yπio_printctx_t *t) {
	return _yΩIO_print_constornot_char16pnt(t, yπio_printctx_va_arg(t, const char16_t *));
}

int _yΩIO_print_constchar32pnt(yπio_printctx_t *t) {
	return _yΩIO_print_constornot_char32pnt(t, yπio_printctx_va_arg(t, const char32_t *));
}


#endif
