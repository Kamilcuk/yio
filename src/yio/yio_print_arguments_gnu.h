/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_YIO_PRINT_ARGUMENTS_GNU_H_
#define YYIO_YIO_YIO_YIO_PRINT_ARGUMENTS_GNU_H_
// NOLINE
{% from "yio/yio_print_arguments.h" import j_yio_macros_args, j_yio_macros_funcs %}
{% for I in j_range(1, j_MLVLS) %}{% call j_APPLY(I) %}
#define YYIO_print_arguments_$1(funcgen, type, fmt{% if I>1 %}{{j_seq(2, I, FMT=",_{}")}}{% endif %}) \
	__extension__({ static const type YYIO_printfuncs[] = { \
{{ j_yio_macros_funcs(I) }}		NULL \
	}; YYIO_printfuncs; }), fmt \
{{ j_yio_macros_args(I) }}	/* */
{% endcall %}{% endfor %}

#endif /* YYIO_YIO_YIO_YIO_PRINT_ARGUMENTS_GNU_H_ */
