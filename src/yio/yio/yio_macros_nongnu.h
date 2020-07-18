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
		&(const yio_printdata_t){0}, (const char *)(fmt)

m4_applyforloopdefine(2, m4_MLVLS, `m4_dnl;
#define _yIO_print_arguments_$1(func_gen, fmt, m4_seqdashcomma(2, $1)) \
	&(const yio_printdata_t){ \
		(const _yIO_printfunc_t[]){ \
			m4_forloopdashX(2, $1, `m4_yio_macros_funcs(X, func_gen)~, ` \
			~) \
			NULL, \
		}, \
m4_ifdef(`m4_DEBUG~, `m4_dnl; \
		(const size_t[]){ \
			m4_forloopdashX(2, $1, `m4_yio_macros_sizes(X)~, ` \
			~) \
			0 \
		}, \
~)m4_dnl; \
	}, \
	(const char *)(fmt) \
	m4_forloopdashX(2, $1, `m4_yio_macros_args(X)~, ` \
	~)m4_dnl;

~)m4_dnl;


~)m4_dnl;
