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

YYIO_XDATA YIO_SDCC_ARGS_AT yio_printdata_t YYIO_sdcc_args[{{ j_MAX_ARGS }}];

#endif
