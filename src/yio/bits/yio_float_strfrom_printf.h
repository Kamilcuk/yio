/**
 * @file yio_float_strfrom_printf.h
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config();
#include "yio_float.h"

m4_applyforeachdefine(`((f), (), (l))~, `m4_dnl;

int _yIO_float_strfrom_printf$1(char *dest, int precision, char type0, _yIO_FLOAT$1 val);
int _yIO_float_astrfrom_printf$1(char **out, int precision, char type, _yIO_FLOAT$1 val);

~) m4_dnl;

