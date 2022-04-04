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

{% from "yio/template/yio_macros.h" import j_yio_macros_funcs, j_yio_macros_fmt_and_args &}

/* print ------------------------------------------------------------------------- */

#define _yΩIO_print_arguments_1(func_gen, fmt)  \
		__extension__({ \
				static const yπio_printdata_t _yΩIO_printdata = {0}; \
				&_yΩIO_printdata;	\
		}), \
		{{j_yio_macros_fmt_and_args(1)}}

{% for I in range(2, j_MLVLS) %}
#define _yΩIO_print_arguments_$1(func_gen, fmt, {{j_seqdashcomma(2, I)}}) \
		__extension__({ \
			static const _yΩIO_printfunc_t _yΩIO_printfuncs[] = { \
				{% for J in range(2, I+1) %}
					{{j_yio_macros_funcs(J, func_gen)}} \
				{% endfor %}
				NULL \
			}; \
			_yΩIO_printfuncs; \
		}), \
		{{j_yio_macros_fmt_and_args(I)}}
{% endfor %}

/* scan ----------------------------------------------------------------- */

#define _yΩIO_scan_arguments_1(func_gen,fmt)  \
		__extension__({ \
			static const yπio_scandata_t _yΩIO_scandata = {0}; \
			&_yΩIO_scandata; \
		}), \
		m4_yio_macros_fmt_and_args(1)

m4_applyforloopdefine(2, m4_MLVLS, «m4_dnl;
#define _yΩIO_scan_arguments_$1(func_gen, fmt, m4_seqdashcomma(2, $1)) \
		__extension__({ \
			static const _yΩIO_scanfunc_t _yΩIO_scanfuncs[] = { \
				m4_forloopdashX(2, $1, «m4_yio_macros_funcs(X, func_gen)», « \
				») \
				NULL \
			}; \
			static const size_t _yΩIO_derefsizes[] = { \
				m4_forloopdashX(2, $1, «m4_yio_macros_derefsizes(X)», « \
				») \
				0 \
			}; \
			static const yπio_scandata_t _yΩIO_scandata = { \
					_yΩIO_scanfuncs, \
					_yΩIO_derefsizes, \
			}; \
			&_yΩIO_scandata; \
		}), \
		m4_yio_macros_fmt_and_args($1)

»)m4_dnl _yIO_scan_arguments_$1 ;

