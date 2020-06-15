/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio_private.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

const char *_yIO_char_to_printstr_in(char dest[5], unsigned char c) {
	if (isprint(c)) {
		dest[0] = c;
	} else {
		dest[0] = '\\';
		dest[1] = 'x';
		const char xdigit[] = "0123456789abcdef";
		dest[2] = xdigit[(c >> 4) & 0xf];
		dest[3] = xdigit[c & 0xf];
	}
	return dest;
}
