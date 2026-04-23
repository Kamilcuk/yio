/**
 * @file
 * @date 2022-04-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include <stdint.h>

int YYIO_print_voidp(yio_printctx_t *t) {
	const void *val = yio_printctx_va_arg(t, void *);
	int err = yio_printctx_init(t);
	if (err) return err;
	const struct yio_printfmt_s *const pf = yio_printctx_get_fmt(t);
	if (pf->type != '\0' && pf->type != 'p') {
		return YIO_ERROR_INVALID_TYPE;
	}
#ifdef UINTPTR_MAX
	const uintptr_t var = (uintptr_t)val;
	return yio_printctx_printf(t, "{:#x}", var);
#else
	return YIO_ERROR_ENOSYS;
#endif
}


