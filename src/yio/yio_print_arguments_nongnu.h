/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_YIO_PRINT_ARGUMENTS_NONGNU_H_
#define YYIO_YIO_YIO_YIO_PRINT_ARGUMENTS_NONGNU_H_
// NOLINE
{% from "yio/yio_print_arguments.h" import j_yio_print_arguments_args, j_yio_macros_args, j_yio_macros_funcs %}
{% for I in j_one_to_n(1, j_MAX_ARGS) %}{% call j_APPLY(I) %}
#define YYIO_print_arguments_$1(funcgen, fmt{{j_yio_print_arguments_args(I)}}) \
	(const yio_printdata_t[]){ \
{{ j_yio_macros_funcs(I) }}		NULL \
	}, fmt \
{{ j_yio_macros_args(I) }}	/* */
{% endcall %}{% endfor %}

#endif /* YYIO_YIO_YIO_YIO_PRINT_ARGUMENTS_NONGNU_H_ */
