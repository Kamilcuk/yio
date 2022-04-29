/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YIO_YIO_PRINT_ARGUMENTS_NONGNU_H_
#define _yIO_YIO_YIO_YIO_PRINT_ARGUMENTS_NONGNU_H_
// NOLINE
{% from "yio/yio_print_arguments.h" import j_yio_macros_args, j_yio_macros_funcs %}
{% for I in j_range(2, j_MLVLS) %}{% call j_APPLY(I) %}
#line
#define _yIO_print_arguments_$1(funcgen, type, fmt{% if I>1 %}{{j_seq(2, I, FMT=",_{}")}}{% endif %}) \
	(const type[]){ \
{{ j_yio_macros_funcs(I) }}		NULL \
	}, fmt \
{{ j_yio_macros_args(I) }}	/* */
{% endcall %}{% endfor %}

#endif /* _yIO_YIO_YIO_YIO_PRINT_ARGUMENTS_NONGNU_H_ */
