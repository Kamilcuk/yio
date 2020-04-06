/**
 * @file yio_ctx.h
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#include "yio.h"
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

/* commonctx --------------------------------------------------------- */

/// Common context information in both printctx and scanctx.
struct _yIO_commonctx_s {
	/// va_list of current argument.
	va_list *va;
	/// Iterator into an array of arguments sizes.
	const size_t *argsizespnt;
};

static inline _yIO_wur _yIO_nn() _yIO_rnn
va_list *_yIO_commonctx_inc_va_list(struct _yIO_commonctx_s *commonctx,
		size_t sizeof_realtype) {
	assert((commonctx) != NULL);
	assert((commonctx)->argsizespnt != NULL);
	assert(*(commonctx)->argsizespnt != 0 ||
			!"You called va_arg too many times");
	assert(*(commonctx)->argsizespnt == sizeof_realtype ||
			!"You called va_arg with invalid type");
	(void)sizeof_realtype;
	commonctx->argsizespnt++;
	return commonctx->va;
}

_yIO_wur _yIO_nn()
int _yIO_commonctx_stdintparam(struct _yIO_commonctx_s *t,
		const char *ptr, const char **endptr, int *res);

#define _yIO_commonctx_va_arg(commonctx, realtype) \
		va_arg(*_yIO_commonctx_inc_va_list(commonctx, sizeof(realtype)), realtype)

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
	const char *fmt;
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


