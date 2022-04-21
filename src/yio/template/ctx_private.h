/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#include "yio/yio_common.h"
#include "ctx.h"
#include "fmt.h"
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

/* printctx ---------------------------------------------------------- */

/**
 * The structure that allows for printing context manipulation.
 */
struct _yΩIO_printctx_s {
	/// va_list of current argument.
	va_list *va;
#ifdef YIO_HAS_POSARG
	/// Copy of va_list when iterating
	va_list *iva;
#endif
	/// Iterator in callback functions.
	const _yΩIO_printfunc_t *ifunc;
	/// The pointer to the data.
	const yπio_printdata_t *data;
	/// The outputting function.
	_yΩIO_printcb_t *out;
	/// User argument for outputting functions.
	void *outarg;
	/// Current iterator in the format string.
	const TCHAR *fmt;
	/// The count of characters written.
	size_t writtencnt;
	/// Description of print formatting.
	struct yπio_printfmt_s pf;
};

static inline _yIO_nn(1, 2, 4, 6)
void _yΩIO_printctx_init(yπio_printctx_t *t,
		_yΩIO_printcb_t *out, void *outarg,
		const yπio_printdata_t *data, const TCHAR *fmt, va_list *va) {
	yπio_printctx_t _yΩIO_printctx = {
		.va = va,
		.fmt = fmt,
		.ifunc = data,
		.data = data,
		.out = out,
		.outarg = outarg,
	};
	*t = _yΩIO_printctx;
}
