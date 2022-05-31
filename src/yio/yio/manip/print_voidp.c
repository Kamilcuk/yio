/**
 * @file
 * @date 2022-04-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"

int YYΩIO_print_voidp(yπio_printctx_t *t) {
	const void *val = yπio_printctx_va_arg(t, void *);
	int err = yπio_printctx_init(t);
	if (err) return err;
	const uintptr_t var = (uintptr_t)val;
	return yπio_printctx_printf(t, TC("{:#x}"), var);
}


