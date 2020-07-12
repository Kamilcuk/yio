/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config_yio() m4_dnl;
#pragma once
#include "yio/yio_common.h"
#include <assert.h>
#include <stdarg.h>

/// Common context information in both printctx and scanctx.
struct _yIO_commonctx_s {
	/// va_list of current argument.
	va_list *va;
	/// Format string.
	const Ychar *fmt;
	/// Copy of va_list when iterating
	va_list *iva;
	/// Copy of format string when iterating.
	const Ychar *ifmt;
#ifdef YIO_CHECK_VA_ARG_SIZES
	/// Iterator into an array of arguments sizes.
	const size_t *argsizespnt;
#endif
};

static inline _yIO_wur _yIO_nn() _yIO_rnn
va_list *_yIO_commonctx_inc_va_list(struct _yIO_commonctx_s *commonctx,
		size_t sizeof_realtype) {
#ifdef YIO_CHECK_VA_ARG_SIZES
	assert((commonctx) != NULL);
	assert((commonctx)->argsizespnt != NULL);
	assert(*(commonctx)->argsizespnt != 0 ||
			!"You called va_arg too many times");
	assert(*(commonctx)->argsizespnt == sizeof_realtype ||
			!"You called va_arg with invalid type");
	(void)sizeof_realtype;
#endif
	commonctx->argsizespnt++;
	return commonctx->va;
}

#define _yIO_commonctx_va_arg(commonctx, realtype) \
		va_arg(*_yIO_commonctx_inc_va_list(commonctx, sizeof(realtype)), realtype)

_yIO_wur _yIO_nn()
int _yIO_commonctx_stdintparam(int (*get_va_arg_int)(void *arg), void *arg,
		const Ychar *ptr, const Ychar **endptr, int *res);
