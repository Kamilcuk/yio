/**
 * @file
 * @date 2026-04-28
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_YIO_PRINT_ARGUMENTS_SDCC_H_
#define YIO_YIO_YIO_PRINT_ARGUMENTS_SDCC_H_
#ifdef __cplusplus
extern "C" {
#endif

// NOLINE
#include "yio/ctx_types.h"
#ifdef __SDCC
#define YIO_SDCC_ARGS_AT YIO_AT(0x1100)
extern YIO_XDATA YIO_SDCC_ARGS_AT yio_printdata_t YIO_sdcc_args[{{ j_MAX_ARGS }}];

{% from "yio/yio_print_arguments.h" import j_yio_print_arguments_args, j_yio_macros_args, j_yio_macros_funcs %}
{% for I in j_one_to_n(1, j_MAX_ARGS) %}{% call j_APPLY(I) %}
#line
#define YIO_print_arguments_$1(funcgen, fmt{{j_yio_print_arguments_args(I)}}) \
	( \
{% for J in j_one_to_n(2, I) %}{% set A = "_"+J|string %}
		YIO_sdcc_args[{{loop.index0}}] = YIO_IFBA62A_IN(YIO_ESC {{A}})(YIO_SECONDX, funcgen)({{A}}, YIO_FIRST YIO_FIRST {{A}}), \
{% endfor %}
		YIO_sdcc_args[{{I - 1}}] = 0, \
		YIO_sdcc_args \
	), fmt \
{{ j_yio_macros_args(I) }}	/* */
{% endcall %}{% endfor %}

#endif // __SDCC

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_PRINT_ARGUMENTS_SDCC_H_
