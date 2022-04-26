/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef _yIO_YIO_YΩIO_CTX_PRIVATE_H_
#define _yIO_YIO_YΩIO_CTX_PRIVATE_H_
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
	/// Current iterator in the format string.
	const TCHAR *fmt;
	/// va_list of current argument.
	va_list *va;
	/// Copy of va_list when iterating
	va_list *startva;
	/// Iterator in callback functions.
	const _yΩIO_printfunc_t *ifunc;
	/// The pointer to the data.
	const yπio_printdata_t *startifunc;
	/// The outputting function.
	_yΩIO_printcb_t *out;
	/// User argument for outputting functions.
	void *outarg;
	/// The count of characters written.
	unsigned writtencnt;
	/// Description of print formatting.
	struct yπio_printfmt_s pf;
	/// How many arguments to skip when outputting current argument.
	unsigned char skip;
};

static inline _yIO_nn(1, 2, 4, 6)
void _yΩIO_printctx_init(yπio_printctx_t *t,
		_yΩIO_printcb_t *out, void *outarg,
		const yπio_printdata_t *data, const TCHAR *fmt, va_list *va, va_list *startva) {
	yπio_printctx_t _yΩIO_printctx = {
		.va = va,
		.startva = startva,
		.fmt = fmt,
		.ifunc = data,
		.startifunc = data,
		.out = out,
		.outarg = outarg,
	};
	*t = _yΩIO_printctx;
}
#endif /* _yIO_YIO_YΩIO_CTX_PRIVATE_H_ */
