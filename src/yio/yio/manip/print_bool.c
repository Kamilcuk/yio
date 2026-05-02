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
#ifndef YIO_ENABLE_LOCALE
#error
#endif
#if YIO_ENABLE_LOCALE
#include <langinfo.h>
#endif

int YYIO_print_bool(yio_printctx_t *t) {
	const bool value = yio_printctx_va_arg_promote(t, _Bool);
	int err = yio_printctx_init(t);
	if (err) return err;
	const struct yio_printfmt_s *fmt = yio_printctx_get_fmt(t);
	if (fmt->type == '\0' || fmt->type == 's') {
#if YIO_ENABLE_LOCALE && defined(YESSTR) && defined(NOSTR)
		if (fmt->grouping == 'L') {
			const char *const loc = nl_langinfo(value ? YESSTR : NOSTR);
			if (loc != NULL && strlen(loc) != 0) {
				return yio_printctx_put(t, loc, strlen(loc));
			}
		}
#endif
		const char *const arr[] = { "false", "true" };
		const unsigned lengths[] = { 5, 4 };
		return yio_printctx_put(t, arr[value], lengths[value]);
	}
	const char zeroone[2] = { '0', '1' };
	return yio_printctx_put_number(t, &zeroone[value], 1, true);
}
