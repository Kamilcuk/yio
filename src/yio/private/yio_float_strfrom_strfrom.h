/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config() m4_dnl;
#pragma once
#include "yio_float.h"

m4_applyforeachdefine(`((f), (), (l))~, `m4_dnl;
#if defined _yIO_HAS_FLOAT$1 && defined _yIO_HAS_strfrom$1
int _yIO_float_astrfrom_strfrom$1(char **out, int precision, char spec, _yIO_FLOAT$1 val);
#endif
~) m4_dnl;
