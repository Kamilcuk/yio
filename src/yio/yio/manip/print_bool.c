/*
 * print_bool.c
 *
 *  Created on: 19 cze 2020
 *      Author: kamil
 */
#define _GNU_SOURCE
#define _XOPEN_SOURCE
#include <stdbool.h>
#include "private.h"
#ifndef YIO_USE_LOCALE
#error
#endif
#if YIO_USE_LOCALE
#include <langinfo.h>
#endif

int YYΩIO_print_bool(yπio_printctx_t *t) {
	const bool value = yπio_printctx_va_arg_promote(t, _Bool);
	int err = yπio_printctx_init(t);
	if (err) return err;
	const struct yπio_printfmt_s *fmt = yπio_printctx_get_fmt(t);
	if (fmt->type == TC('\0') || fmt->type == TC('s')) {
#if YIO_USE_LOCALE && defined(YESSTR) && defined(NOSTR)
		if (fmt->grouping == TC('L')) {
			const char *const loc = nl_langinfo(value ? YESSTR : NOSTR);
			if (loc != NULL && strlen(loc) != 0) {
				return yπio_printctx_put(t, loc, strlen(loc));
			}
		}
#endif
		const TCHAR *const arr[] = { TC("false"), TC("true") };
		const unsigned lengths[] = { 5, 4 };
		return yπio_printctx_putπ(t, arr[value], lengths[value]);
	}
	const TCHAR zeroone[2] = { TC('0'), TC('1') };
	return yπio_printctx_putπ_number(t, &zeroone[value], 1, true);
}
