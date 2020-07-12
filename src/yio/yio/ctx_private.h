/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config_yio_template(`m4_dnl);
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
	const struct _yΩIO_printdata_s * const data;
	/// The outputting function.
	_yΩIO_printcb_t * const out;
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
yπio_printctx_t _yΩIO_printctx_init(
		_yΩIO_printcb_t *out, void *outarg,
		const yπio_printdata_t *data,
		va_list *va) {
	return (yπio_printctx_t){
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
	/// Iterator into an array of dereferenced arguments sizes.
	const size_t *argpntsizespnt;
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
		.argpntsizespnt = data->argpntsizes,
		.inarg = inarg,
		.fmt = data->fmt,
	};
}

~)m4_dnl;
