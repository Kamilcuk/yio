/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
// M4_NOSYNCLINES
#pragma once

m4_divert(-1)
m4_include(«yio/template/yio_macros.h»)
m4_divert(0)m4_dnl;

/* print ------------------------------------------------------------------------- */

#define _yΩIO_print_arguments_1(func_gen, fmt)  \
		__extension__({ \
				static const yπio_printdata_t _yΩIO_printdata = {0}; \
				&_yΩIO_printdata;	\
		}), \
		m4_yio_macros_fmt_and_args(1)

m4_applyforloopdefine(2, m4_MLVLS, «m4_dnl;
#define _yΩIO_print_arguments_$1(func_gen, fmt, m4_seqdashcomma(2, $1)) \
		__extension__({ \
			static const _yΩIO_printfunc_t _yΩIO_printfuncs[] = { \
				m4_forloopdashX(2, $1, «m4_yio_macros_funcs(X, func_gen)», « \
				») \
				NULL \
			}; \
			_yΩIO_printfuncs; \
		}), \
		m4_yio_macros_fmt_and_args($1)

») m4_dnl _yIO_print_arguments_$1 ;

