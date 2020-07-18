/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config_yio_template(`m4_dnl);
#pragma once

m4_divert(-1)
m4_include(`yio/yio/yio_macros.h~)
m4_divert(0)m4_dnl;

/* print ------------------------------------------------------------------------- */

#define _yIO_print_arguments_1(func_gen, fmt)  \
		__extension__({ \
				static const yio_printdata_t _yIO_printdata = {0}; \
				&_yIO_printdata;	\
		}), (const char *)(fmt)

m4_applyforloopdefine(2, m4_MLVLS, `m4_dnl;
#define _yIO_print_arguments_$1(func_gen, fmt, m4_seqdashcomma(2, $1)) \
	__extension__({ \
		static const _yIO_printfunc_t _yIO_printfuncs[] = { \
			m4_forloopdashX(2, $1, `m4_yio_macros_funcs(X, func_gen)~, ` \
			~) \
			NULL, \
		}; \
m4_ifdef(`m4_DEBUG~, `m4_dnl; \
		static const size_t _yIO_printsizes[] = { \
			m4_forloopdashX(2, $1, `m4_yio_macros_argsizes(X)~, ` \
			~) \
			0 \
		}; \
~)m4_dnl; \
		static const yio_printdata_t _yIO_printdata = { \
			_yIO_printfuncs, \
m4_ifdef(`m4_DEBUG~, `m4_dnl; \
			_yIO_printsizes, \
~)m4_dnl; \
		}; \
		&_yIO_printdata; \
	}), \
	(const char *)(fmt) \
	m4_forloopdashX(2, $1, `m4_yio_macros_args(X)~, ` \
	~)m4_dnl;

~)m4_dnl;

~)m4_dnl;
