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

yio_printdata_t YYIO_sdcc_args[YIO_SDCC_MAX_ARGS];

{% from "yio/yio_print_arguments_sdcc.h" import j_dec_YYIO_sdcc_args_init %}
{{ j_dec_YYIO_sdcc_args_init() }} {
{%- for J in j_one_to_n(2, j_MLVLS) -%}
	YYIO_sdcc_args[{{ loop.index0 }}] = _{{ J }};
{%- endfor %}
	return YYIO_sdcc_args;
}

#endif
