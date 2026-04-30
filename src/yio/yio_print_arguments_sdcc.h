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

#define YIO_SDCC_MAX_ARGS {{ j_MLVLS - 1 }}
extern yio_printdata_t YYIO_sdcc_args[YIO_SDCC_MAX_ARGS];
{% macro j_dec_YYIO_sdcc_args_init() %}
const yio_printdata_t *YYIO_sdcc_args_init(
{%- for J in j_one_to_n(2, j_MLVLS) -%}
const yio_printdata_t _{{ J }}{% if not loop.last %}, {% endif %}
{%- endfor -%}
)
{%- endmacro %}
{{ j_dec_YYIO_sdcc_args_init() }};

{% from "yio/yio_print_arguments.h" import j_yio_print_arguments_args, j_yio_macros_args, j_yio_macros_funcs %}
{% for I in j_one_to_n(1, j_MLVLS) %}{% call j_APPLY(I) %}
#line
#define YYIO_print_arguments_$1(funcgen, fmt{{j_yio_print_arguments_args(I)}}) \
	(const yio_printdata_t*)YYIO_sdcc_args_init( \
{{ j_yio_macros_funcs(I) }}  \
{% for J in j_one_to_n(I + 1, j_MLVLS) %}
	0{% if not loop .last %}, {% endif %} \
{% endfor %}
	), fmt \
{{ j_yio_macros_args(I) }}	/* */
{% endcall %}{% endfor %}

#endif /* YYIO_YIO_YIO_YIO_PRINT_ARGUMENTS_SDCC_H_ */
