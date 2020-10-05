/**
 * @file
 * @date 2020-08-17
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_float.h"
#include "yio_float_strfrom_stupid.h"
#include <stddef.h>

#ifdef __GNUC__
#define _yIO_weak __attribute__((__weak__))
#endif

m4_applyforeachdefine(((f),(d),(l)), m4_synclines(1)`m4_dnl

#define TYPE _yIO_FLOAT$1

#ifdef __GNUC__
_yIO_weak
int _yIO_user_f2fixed$1(char **resultp, size_t *lengthp,
		TYPE f, uint32_t precision) { return NULL; }
_yIO_weak
int _yIO_user_f2exp$1(char **resultp, size_t *lengthp,
		TYPE f, uint32_t precision) { return NULL; }
#endif

int _yIO_float_astrfrom_user$1(char **resultp, size_t *lengthp,
		int precision, TYPE f) {


}

~)m4_dnl;
