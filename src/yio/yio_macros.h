/**
 * @file yio_macros.h
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include <yio/yio_macros_priv.h>

/* Printers Generic Choosers ------------------------------------------------------------------------------------ */

#define _yIO_print_arguments_1(func_gen,fmt)  &(const yio_printdata_t){(fmt),NULL,NULL,NULL}
#define _yIO_print_arguments_2(func_gen,fmt,...) \
	&(const yio_printdata_t){ \
		(fmt), \
		(const _yIO_printfunc_t[]){ \
			_yIO_JFB62TE((), (,), _yIO_JCIFSFT, func_gen, __VA_ARGS__) \
			NULL \
		}, \
		(const size_t[]){ \
			_yIO_JFB62TE((), (,), _yIO_JSDFEF, _yIO_SIZEOFDECAY, __VA_ARGS__) \
			0 \
		}, \
		(const char*const[]){ \
			_yIO_JFB62TE((), (,), _yIO_JSTRFEF, _yIO_STRINGIFY, __VA_ARGS__) \
			NULL, \
		}, \
	} \
	_yIO_JFB62TE((,), (), _yIO_JIGNORE1, _yIO_ESC, __VA_ARGS__)
#define _yIO_print_arguments_N(m4_seqdashcomma(0, 62), N, ...)  \
		_yIO_print_arguments_##N


#define _yIO_scan_arguments_1(func_gen,fmt)  &(const yio_scandata_t){(fmt),NULL,NULL,NULL}
#define _yIO_scan_arguments_2(func_gen,fmt,...)  \
	&(const yio_scandata_t){ \
		(fmt), \
		(const _yIO_scanfunc_t[]){ \
			_yIO_JFB62TE((), (,), _yIO_JCIFSFT, func_gen, __VA_ARGS__) \
			NULL \
		}, \
		(const size_t[]){ \
			_yIO_JFB62TE((), (,), _yIO_JSFEF, _yIO_SIZEOF_DEREF, __VA_ARGS__) \
			0 \
		}, \
		(const size_t[]){ \
			_yIO_JFB62TE((), (,), _yIO_JSDFEF, _yIO_SIZEOFDECAY, __VA_ARGS__) \
			0 \
		}, \
		(const char*const[]){ \
			_yIO_JFB62TE((), (,), _yIO_JSTRFEF, _yIO_STRINGIFY, __VA_ARGS__) \
			NULL, \
		}, \
	} \
	_yIO_JFB62TE((,), (), _yIO_JIGNORE1, _yIO_ESC, __VA_ARGS__)
#define _yIO_scan_arguments_N(m4_seqdashcomma(0, 62), N, ...)  \
	_yIO_scan_arguments_##N

