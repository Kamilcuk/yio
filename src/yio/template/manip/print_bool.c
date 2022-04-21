/*
 * print_bool.c
 *
 *  Created on: 19 cze 2020
 *      Author: kamil
 */
#include <stdbool.h>
#include "private.h"

int _yΩIO_print_bool(yπio_printctx_t *t) {
	const bool value = yπio_printctx_va_arg_num(t, _Bool);
	int err = yπio_printctx_init(t);
	if (err) return err;

	const struct yπio_printfmt_s *fmt = yπio_printctx_get_fmt(t);
	if (fmt->type == 's') {
		const TCHAR *arr[] = { TC("false"), TC("true") };
		const unsigned lengths[] = { 5, 4 };
		return yπio_printctx_putπ(t, arr[value], lengths[value]);
	}
	const TCHAR zeroone[2] = { TC('0'), TC('1') };
	return yπio_printctx_putπ(t, &zeroone[value], 1);
}
