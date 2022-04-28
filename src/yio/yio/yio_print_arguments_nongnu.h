/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YΩIO_YIO_PRINT_ARGUMENTS_NONGNU_H_
#define _yIO_YIO_YΩIO_YIO_PRINT_ARGUMENTS_NONGNU_H_

{% from "yio/yio/yio_print_arguments.h" import j_yio_macros_fmt_and_args, j_yio_macros_funcs %}

#define _yΩIO_print_arguments_1(func_gen, fmt) \
		(yπio_printdata_t[]){ NULL }, \
		{{ j_yio_macros_fmt_and_args(0) }}

{% for I in j_range(2, j_MLVLS) %}{% call j_APPLY(I) %}
#line
#define _yΩIO_print_arguments_$1(func_gen, fmt{{j_seq(2, I, FMT=",_{}")}}) \
		(yπio_printdata_t[]){ \
			{% for J in j_range(2, I) %}
			{{ j_yio_macros_funcs("_"+J|string, "func_gen") }} \
			{% endfor %}
			NULL \
		}, \
		{{ j_yio_macros_fmt_and_args(I) }}
{% endcall %}{% endfor %}

#endif /* _yIO_YIO_YΩIO_YIO_PRINT_ARGUMENTS_NONGNU_H_ */
