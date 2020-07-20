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
		}), \
		m4_yio_macros_fmt_and_args(1)

m4_applyforloopdefine(2, m4_MLVLS, `m4_dnl;
#define _yIO_print_arguments_$1(func_gen, fmt, m4_seqdashcomma(2, $1)) \
		__extension__({ \
			static const _yIO_printfunc_t _yIO_printfuncs[] = { \
				m4_forloopdashX(2, $1, `m4_yio_macros_funcs(X, func_gen)~, ` \
				~) \
				NULL \
			}; \
			_yIO_printfuncs; \
		}), \
		m4_yio_macros_fmt_and_args($1)

~) m4_dnl _yIO_print_arguments_$1 ;

/* scan ----------------------------------------------------------------- */

#define _yIO_scan_arguments_1(func_gen,fmt)  \
		__extension__({ \
			static const yio_scandata_t _yIO_scandata = {0}; \
			&_yIO_scandata; \
		}), \
		m4_yio_macros_fmt_and_args(1)

m4_applyforloopdefine(2, m4_MLVLS, `m4_dnl;
#define _yIO_scan_arguments_$1(func_gen, fmt, m4_seqdashcomma(2, $1)) \
		__extension__({ \
			static const _yIO_scanfunc_t _yIO_scanfuncs[] = { \
				m4_forloopdashX(2, $1, `m4_yio_macros_funcs(X, func_gen)~, ` \
				~) \
				NULL \
			}; \
			static const size_t _yIO_derefsizes[] = { \
				m4_forloopdashX(2, $1, `m4_yio_macros_derefsizes(X)~, ` \
				~) \
				0 \
			}; \
			static const yπio_scandata_t _yIO_scandata = { \
					_yIO_scanfuncs, \
					_yIO_derefsizes, \
			}; \
			&_yIO_scandata; \
		}), \
		m4_yio_macros_fmt_and_args($1)

~)m4_dnl _yIO_scan_arguments_$1 ;

~)m4_dnl m4_config_yio_template ;
