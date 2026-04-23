/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#if YIO_HAS_UCHAR_H

int YYIO_print_constchar16pnt(yio_printctx_t *t) {
	const char16_t *const str1 = yio_printctx_va_arg(t, const char16_t *);
	const char16_t *const str = str1 == NULL ? u"(null)" : str1;
	int err = yio_printctx_init(t);
	if (err) return err;
	const struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	const size_t len = YYIO_c16strnlen(str, pf->precision >= 0 ? (size_t)pf->precision : SIZE_MAX);
	const char *dest = NULL;
	size_t dest_len = 0;
	err = YYIO_strconv_c16str_to_str(str, len, &dest, &dest_len);
	if (err) return err;
	err = yio_printctx_put(t, dest, dest_len);
	YYIO_strconv_free_c16str_to_str(str, dest);
	return err;
}

int YYIO_print_constchar32pnt(yio_printctx_t *t) {
	const char32_t *const str1 = yio_printctx_va_arg(t, const char32_t *);
	const char32_t *const str = str1 == NULL ? U"(NULL)" : str1;
	int err = yio_printctx_init(t);
	if (err) return err;
	const struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	const size_t len = YYIO_ustrnlen(str, pf->precision >= 0 ? (size_t)pf->precision : SIZE_MAX);
	const char *dest = NULL;
	size_t dest_len = 0;
	err = YYIO_strconv_ustr_to_str(str, len, &dest, &dest_len);
	if (err) return err;
	err = yio_printctx_put(t, dest, dest_len);
	YYIO_strconv_free_ustr_to_str(str, dest);
	return err;
}


#endif
