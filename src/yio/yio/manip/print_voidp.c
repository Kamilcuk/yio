/**
 * @file
 * @date 2022-04-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "print_int.h"
#include <stdint.h>

#if YIO_HAS_LLONG
#define YIO_print_in(t, arg)\
	_Generic(arg \
					,unsigned int: YIO_print_uint_in \
					,unsigned long: YIO_print_ulong_in \
					,unsigned long long: YIO_print_ullong_in \
	)(t, arg, 0)
#else
#define YIO_print_in(t, arg)\
	_Generic(arg \
					,unsigned int: YIO_print_uint_in \
					,unsigned long: YIO_print_ulong_in \
	)(t, arg, 0)
#endif

int YIO_print_voidp(yio_printctx_t *t) {
	const void *val = yio_printctx_va_arg(t, void *);
	int err = yio_printctx_init(t);
	if (err) return err;
	const struct yio_printfmt_s *const pf = yio_printctx_get_fmt(t);
	if (pf->type != '\0' && pf->type != 'p') {
		return YIO_ERROR_INVALID_TYPE;
	}
#ifdef __SDCC
	typedef uint32_t T;
#elif defined(UINTPTR_MAX) && UINTPTR_MAX
	typedef uintptr_t T;
#elif defined(UINTMAX_MAX) && UINTMAX_MAX
	typedef uintmax_t T;
#elif YIO_HAS_LLONG
	typedef unsigned long long T;
#else
	typedef unsigned long T;
#endif
	err = yio_printctx_put(t, "0x", 2);
	if (err) return err;
	const T var = (T)val;
	t->pf.type = 'x'; // setup integer printing as x.
	return YIO_print_in(t, var);
}


