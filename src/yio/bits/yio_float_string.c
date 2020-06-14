/**
 * @file yio_float_string.c
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_float_string.h"
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

m4_applyforeachdefine(`((f), (), (l))~, `m4_dnl;

int _yIO_float_strfrom_to_astrfrom$1(char **out, int precision, char spec, _yIO_FLOAT$1 val,
		int (*strfrompnt)(char *out, int precision, char spec, _yIO_FLOAT$1 val)) {
	const int len = strfrompnt(NULL, precision, spec, val);
	if (len < 0) {
		*out = NULL;
		return len;
	}
	char * const buf = malloc(len + 1);
	if (buf == NULL) {
		*out = NULL;
		return -ENOMEM;
	}
	strfrompnt(buf, precision, spec, val);
	*out = buf;
	return 0;
}

~) m4_dnl ;

