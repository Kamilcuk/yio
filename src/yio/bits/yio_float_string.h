/**
 * @file yio_float_string.h
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "yio_float.h"

m4_applyforeachdefine(`((f), (), (l))~, `m4_dnl;

int _yIO_float_strfrom_to_astrfrom$1(char **out, int precision, char spec, _yIO_FLOAT$1 val,
		int (*strfrompnt)(char *out, int precision, char spec, _yIO_FLOAT$1 val));

~) m4_dnl ;
