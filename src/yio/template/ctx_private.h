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
	const yπio_printdata_t * const data;
	/// The outputting function.
	_yΩIO_printcb_t * const out;
	/// User argument for outputting functions.
	void * const outarg;
	/// Current iterator in the format string.
	const Ychar *fmt;
	/// The count of characters written.
	size_t writtencnt;
	/// Description of print formatting.
	struct yπio_printfmt_s pf;
};

static inline _yIO_wur _yIO_nn(1, 2, 3)
yπio_printctx_t _yΩIO_printctx_init(
		_yΩIO_printcb_t *out, void *outarg,
		const yπio_printdata_t *data,
		va_list *va) {
	yπio_printctx_t _yΩIO_printctx = {
		.data = data,
		.c.va = va,
		.out = out,
		.outarg = outarg,
		.ifunc = data,
	};
	_yΩIO_printctx.fmt = va_arg(*va, const Ychar *);
	m4_ifdef(`m4_DEBUG~,`
	// Debug build also has size_t argument passed
	_yΩIO_printctx.c.argsizespnt = va_arg(*va, const size_t*);
	~)
	return _yΩIO_printctx;
}

/* scanctx ----------------------------------------------------------- */

/**
 * The structure that allows for scanning context manipulation.
 */
struct _yΩIO_scanctx_s {
	/// Common.
	struct _yΩIO_commonctx_s c;
	/// Iterator in callback functions.
	const _yΩIO_scanfunc_t *ifunc;
	/// The pointer to the data.
	const yπio_scandata_t *data;
	/// Current iterator in the format string.
	const Ychar *fmt;
	/// The inputting function.
	_yΩIO_scancb_t *in;
	/// User argument for inputting function.
	void *inarg;
	/// Iterator into an array of dereferenced arguments sizes.
	const size_t *argpntsizespnt;
	/// The count of characters written or read.
	size_t scannedcnt;
	/// The formatting options
	struct yπio_scanfmt_s sf;
	/// The last character read.
	Yint ch;
	/// unin
	bool unin;
};

static inline _yIO_wur _yIO_nn(1, 3, 4)
yπio_scanctx_t _yΩIO_scanctx_init(
		_yΩIO_scancb_t *in, void *inarg,
		const yπio_scandata_t *data,
		va_list *va) {
	yπio_scanctx_t _yΩIO_scanctx = {
		.data = data,
		.c.va = va,
		.ifunc = data->funcs,
		.in = in,
		.argpntsizespnt = data->argpntsizes,
		.inarg = inarg,
	};
	_yΩIO_scanctx.fmt = va_arg(*va, const Ychar *);
	m4_ifdef(`m4_DEBUG~,`
	// Debug build also has size_t argument passed
	_yΩIO_scanctx.c.argsizespnt = va_arg(*va, const size_t *);
	~)
	return _yΩIO_scanctx;
}


