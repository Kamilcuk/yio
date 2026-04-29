/**
 * @file
 * @date 2026-04-28
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_YIO_PRINT_ARGUMENTS_SDCC_H_
#define YYIO_YIO_YIO_YIO_PRINT_ARGUMENTS_SDCC_H_
// NOLINE
{% from "yio/yio_print_arguments.h" import j_yio_macros_args %}

#define YIO_SDCC_MAX_ARGS {{ j_MLVLS + 1 }}
extern const void * yyio_sdcc_args[YIO_SDCC_MAX_ARGS];

{% for I in j_one_to_n(2, j_MLVLS) %}
const void * yyio_init_args_{{ I }}(
{%- for J in j_one_to_n(2, I) -%}
const void * _{{ J }}{% if not loop.last %}, {% endif %}
{%- endfor -%}
);
{% endfor %}

{% for I in j_one_to_n(2, j_MLVLS) %}{% call j_APPLY(I) %}
#line
#define YYIO_print_arguments_$1(funcgen, type, fmt{% if I>1 %}{{j_seq(2, I, FMT=",_{}")}}{% endif %}) \
	(const type*)yyio_init_args_$1( \
{% for J in j_one_to_n(2, I) %}{% set A = "_"+J|string %} \
		YYIO_IFBA62A_IN(YYIO_ESC {{A}})(YYIO_SECONDX, funcgen)({{A}}, YYIO_FIRST YYIO_FIRST {{A}}){% if not loop.last %},{% endif %} \
{% endfor %} \
	), fmt \
{{ j_yio_macros_args(I) }}	/* */
{% endcall %}{% endfor %}

#endif /* YYIO_YIO_YIO_YIO_PRINT_ARGUMENTS_SDCC_H_ */
