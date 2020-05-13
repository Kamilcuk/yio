/**
 * @file ctx.h
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config();
#pragma once
#include "yio/yio_common.h"
#include "commonctx.h"
#include "ctx_public.h"
#include "fmt_public.h"
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
struct _yIO_printctx_s {
	/// Common.
	struct _yIO_commonctx_s c;
	/// Iterator in callback functions.
	const _yIO_printfunc_t *ifunc;
	/// The pointer to the data.
	const struct _yIO_printdata_s * const data;
	/// The outputting function.
	_yIO_printcb_t * const out;
	/// User argument for outputting functions.
	void * const outarg;
	/// Current iterator in the format string.
	const Ychar *fmt;
	/// The count of characters written.
	size_t writtencnt;
	/// Description of print formatting.
	struct yio_printfmt_s pf;
};

static inline _yIO_wur _yIO_nn(1, 2, 3)
yio_printctx_t _yIO_printctx_init(
		_yIO_printcb_t *out, void *outarg,
		const yio_printdata_t *data,
		va_list *va) {
	return (yio_printctx_t){
		.data = data,
		.c.va = va,
		.c.argsizespnt = data->argsizes,
		.ifunc = data->funcs,
		.out = out,
		.outarg = outarg,
		.fmt = data->fmt,
	};
}

/* scanctx ----------------------------------------------------------- */

/**
 * The structure that allows for scanning context manipulation.
 */
struct _yIO_scanctx_s {
	/// Common.
	struct _yIO_commonctx_s c;
	/// Iterator in callback functions.
	const _yIO_scanfunc_t *ifunc;
	/// The pointer to the data.
	const yio_scandata_t *data;
	/// Current iterator in the format string.
	const char *fmt;
	/// The inputting function.
	_yIO_scancb_t *in;
	/// User argument for inputting function.
	void *inarg;
	/// The count of characters written or read.
	size_t scannedcnt;
	/// The formatting options
	struct yio_scanfmt_s sf;
	/// The last character read.
	int ch;
	/// unin
	bool unin;
};

static inline _yIO_wur _yIO_nn(1, 3, 4)
yio_scanctx_t _yIO_scanctx_init(
		_yIO_scancb_t *in, void *inarg,
		const yio_scandata_t *data,
		va_list *va) {
	return (yio_scanctx_t){
		.data = data,
		.c.va = va,
		.c.argsizespnt = data->argsizes,
		.ifunc = data->funcs,
		.in = in,
		.inarg = inarg,
		.fmt = data->fmt,
	};
}


