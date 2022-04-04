/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#include "yio/yio_common.h"
#include "commonctx.h"
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
	/// Common.
	struct _yΩIO_commonctx_s c;
	/// Iterator in callback functions.
	const _yΩIO_printfunc_t *ifunc;
	/// The pointer to the data.
	const yπio_printdata_t *data;
	/// The outputting function.
	_yΩIO_printcb_t *out;
	/// User argument for outputting functions.
	void *outarg;
	/// Current iterator in the format string.
	const Ychar *fmt;
	/// The count of characters written.
	size_t writtencnt;
	/// Description of print formatting.
	struct yπio_printfmt_s pf;
};

static inline _yIO_nn(1, 2, 4, 6)
void _yΩIO_printctx_init(yπio_printctx_t *t,
		_yΩIO_printcb_t *out, void *outarg,
		const yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	yπio_printctx_t _yΩIO_printctx = {
		.data = data,
		.c.va = va,
		.out = out,
		.outarg = outarg,
		.ifunc = data,
		.fmt = fmt,
	};
	m4_ifdef(«m4_DEBUG»,«
	// Debug build also has size_t argument passed
	_yΩIO_printctx.c.argsizespnt = va_arg(*va, const size_t*);
	»)
	*t = _yΩIO_printctx;
}
