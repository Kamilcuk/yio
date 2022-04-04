/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "yio/yio_common.h"
#include <assert.h>
#include <stdarg.h>

/// Common context information
struct _yΩIO_commonctx_s {
	/// va_list of current argument.
	va_list *va;
	/// Format string.
	const Ychar *fmt;
	/// Copy of va_list when iterating
	va_list *iva;
	/// Copy of format string when iterating.
	const Ychar *ifmt;
};

static inline _yIO_wur _yIO_nn() _yIO_rnn
va_list *_yΩIO_commonctx_inc_va_list(struct _yΩIO_commonctx_s *commonctx,
		size_t sizeof_realtype) {
	(void)sizeof_realtype;
	return commonctx->va;
}

#define _yΩIO_commonctx_va_arg(commonctx, realtype) \
		va_arg(*_yΩIO_commonctx_inc_va_list(commonctx, sizeof(realtype)), realtype)

_yIO_wur _yIO_nn()
int _yΩIO_commonctx_stdintparam(int (*get_va_arg_int)(void *arg), void *arg,
		const Ychar *ptr, const Ychar **endptr, int *res);


