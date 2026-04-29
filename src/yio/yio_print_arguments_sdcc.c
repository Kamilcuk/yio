/**
 * @file
 * @date 2026-04-28
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */

#include "yio_print_arguments_sdcc.h"

#ifdef __SDCC

const void *YYIO_sdcc_args[YIO_SDCC_MAX_ARGS];

{% from "yio/yio_print_arguments_sdcc.h" import j_dec_YYIO_init_args %}
{% for I in j_one_to_n(2, j_MLVLS) %}
{{ j_dec_YYIO_init_args(I) }} {
{%- for J in j_one_to_n(2, I) %}
	YYIO_sdcc_args[{{ loop.index0 }}] = _{{ J }};
{%- endfor %}
	YYIO_sdcc_args[{{ I - 1 }}] = 0;
	return YYIO_sdcc_args;
}
{% endfor %}

#endif
