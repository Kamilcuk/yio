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
		(const _yIO_printfunc_t[]){0}, \
		m4_yio_macros_fmt_and_args(1)

m4_applyforloopdefine(2, m4_MLVLS, `m4_dnl;
#define _yIO_print_arguments_$1(func_gen, fmt, m4_seqdashcomma(2, $1)) \
		(const _yIO_printfunc_t[]){ \
			m4_forloopdashX(2, $1, `m4_yio_macros_funcs(X, func_gen)~, ` \
			~) \
			NULL \
		}, \
		m4_yio_macros_fmt_and_args($1)

~)m4_dnl;

/* scan ------------------------------------------------------------------------------- */

#define _yIO_scan_arguments_1(func_gen,fmt)  \
		&(const yio_scandata_t){0}, \
		m4_yio_macros_fmt_and_args(1)


m4_applyforloopdefine(2, m4_MLVLS, `m4_dnl;
#define _yIO_scan_arguments_$1(func_gen, fmt, m4_seqdashcomma(2, $1)) \
		&(const yπio_scandata_t){ \
			(const _yIO_scanfunc_t[]){ \
				m4_forloopdashX(2, $1, `m4_yio_macros_funcs(X, func_gen)~, ` \
				~) \
				NULL \
			}, \
			(const size_t[]){ \
				m4_forloopdashX(2, $1, `m4_yio_macros_derefsizes(X)~, ` \
				~) \
				0 \
			} \
		}, \
		m4_yio_macros_fmt_and_args($1)

~) m4_dnl _yIO_scan_arguments_$1 ;

~)m4_dnl;

