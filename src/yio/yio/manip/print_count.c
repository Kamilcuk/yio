/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config_yio_template(`m4_dnl);
#include "private.h"

int _yIO_print_count(yio_printctx_t *t) {
	int *arg = yio_printctx_va_arg(t, int *);
	int err = yπio_printctx_init(t);
	if (err) return err;
	const size_t writtencnt = t->writtencnt;
	if (writtencnt > INT_MAX) {
		*arg = INT_MAX;
	} else {
		*arg = writtencnt;
	}
	return 0;
}

~)m4_dnl;
