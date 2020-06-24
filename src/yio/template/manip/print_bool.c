/*
 * print_bool.c
 *
 *  Created on: 19 cze 2020
 *      Author: kamil
 */
#include <stdbool.h>
#include "private.h"

int _yΩIO_print_bool(yπio_printctx_t *t) {
	bool value = yπio_printctx_va_arg2(t, _Bool, int);
	const Ychar *arr[] = { Yc("true"), Yc("false") };
	const size_t arrlens[] = { 4, 5 };
	if (yπio_printctx_get_fmt(t)->type == '\0') {
		return yπio_printctx_out(t, arr[value], arrlens[value]);
	}
	const Ychar zeroone[] = { Yc('0'), Yc('1') };
	return yπio_printctx_out(t, &zeroone[value], 1);
}

