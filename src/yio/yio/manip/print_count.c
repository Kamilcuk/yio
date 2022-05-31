/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"

int YYΩIO_print_count(yπio_printctx_t *t) {
	int *arg = yπio_printctx_va_arg(t, int *);
	int err = yπio_printctx_init(t);
	if (err) return err;
	const size_t writtencnt = t->writtencnt;
	if (writtencnt > INT_MAX) {
		*arg = INT_MAX;
	} else {
		*arg = (int)writtencnt;
	}
	return 0;
}

