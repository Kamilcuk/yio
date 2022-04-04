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

{% from "yio/template/yio_macros.h" import %}

/* print ------------------------------------------------------------------------- */

#define _yΩIO_print_arguments_1(func_gen, fmt)  \
		(const _yΩIO_printfunc_t[]){0}, \
		{{j_yio_macros_fmt_and_args(1)}}

{% for I in range(2, j_MLVLS) %}
#define _yΩIO_print_arguments_$1(func_gen, fmt, {{j_seqdashcomma(2, I)}}) \
		(const _yΩIO_printfunc_t[]){ \
			{% for J in range(2, I+1) %}
				{{m4_yio_macros_funcs(X, "func_gen")}}} \
			{% endfor %}
			NULL \
		}, \
		{{j_yio_macros_fmt_and_args(I)}}
{% endfor %}

/* scan ------------------------------------------------------------------------------- */

#define _yΩIO_scan_arguments_1(func_gen,fmt)  \
		&(const yπio_scandata_t){0}, \
		{{j_yio_macros_fmt_and_args(1)}}


{% for I in range(2, j_MLVLS) %}
#define _yΩIO_scan_arguments_$1(func_gen, fmt, m4_seqdashcomma(2, $1)) \
		&(const yπio_scandata_t){ \
			(const _yΩIO_scanfunc_t[]){ \
				{% for J in range(2, I+1) %}
					{{m4_yio_macros_funcs(X, "func_gen")}}} \
				{% endfor %}
				NULL \
			}, \
			(const size_t[]){ \
				m4_forloopdashX(2, $1, «m4_yio_macros_derefsizes(X)», « \
				») \
				0 \
			} \
		}, \
		m4_yio_macros_fmt_and_args($1)

») m4_dnl _yIO_scan_arguments_$1 ;


