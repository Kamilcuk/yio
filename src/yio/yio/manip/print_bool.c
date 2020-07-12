/*
 * print_bool.c
 *
 *  Created on: 19 cze 2020
 *      Author: kamil
 */
m4_config_yio_template(`m4_dnl);
#include <stdbool.h>
#include "private.h"

int _yΩIO_print_bool(yπio_printctx_t *t) {
	const bool value = yπio_printctx_va_arg_num(t, _Bool);
	int err = yπio_printctx_init(t);
	if (err) return err;
	const Ychar *arr[] = { Yc("true"), Yc("false") };
	if (yπio_printctx_get_fmt(t)->type == '\0') {
		return yπio_printctx_puts(t, arr[value]);
	}
	const Ychar zeroone[][2] = { Yc("0"), Yc("1") };
	return yπio_printctx_puts(t, zeroone[value]);
}

~)m4_dnl;
