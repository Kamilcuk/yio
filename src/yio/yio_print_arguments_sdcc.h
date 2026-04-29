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
#include "yio/ctx_types.h"

#define YIO_SDCC_MAX_ARGS {{ j_MLVLS + 1 }}
extern const void *YYIO_sdcc_args[YIO_SDCC_MAX_ARGS];

{% macro j_dec_YYIO_init_args(I) %}
const void *YYIO_init_args_{{ I }}(
{%- for J in j_one_to_n(2, I) -%}
const void *_{{ J }}{% if not loop.last %}, {% endif %}
{%- endfor -%}
)
{%- endmacro %}

{% for I in j_one_to_n(2, j_MLVLS) %}
{{ j_dec_YYIO_init_args(I) }};
{% endfor %}

{% from "yio/yio_print_arguments.h" import j_yio_print_arguments_args, j_yio_macros_args, j_yio_macros_funcs %}
{% for I in j_one_to_n(2, j_MLVLS) %}{% call j_APPLY(I) %}
#line
#define YYIO_print_arguments_$1(funcgen, fmt{{j_yio_print_arguments_args(I)}}) \
	(const yio_printdata_t*)YYIO_init_args_$1( \
{{ j_yio_macros_funcs(I, 0) }}  \
	), fmt \
{{ j_yio_macros_args(I) }}	/* */
{% endcall %}{% endfor %}

#endif /* YYIO_YIO_YIO_YIO_PRINT_ARGUMENTS_SDCC_H_ */
