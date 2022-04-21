/**
 * @file
 * @date 2020-lip-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_time.h"
#include "private.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <errno.h>

int _yIO_astrftime_nonzero(char **strp, size_t bufsize, const char *fmt, const struct tm *tm) {
	char *buf = *strp;
	errno = 0;
	size_t count = strftime(buf, bufsize, fmt, tm);
	if (count == 0) {
		if (errno != 0) {
			return YIO_ERROR_STRFTIME;
		}
		buf = NULL;
		while (1) {
			// Golden ratio resize
			assert((size_t)SIZE_MAX / 25u > bufsize);
			bufsize *= 25;
			bufsize /= 16;

			void *p = realloc(buf, bufsize);
			if (p == NULL) {
				free(buf);
				return YIO_ERROR_ENOMEM;
			}
			buf = p;

			errno = 0;
			count = strftime(buf, bufsize, fmt, tm);
			if (count != 0) {
				break;
			} else if (errno != 0) {
				return YIO_ERROR_STRFROM;
			}
		}
	}
	//
	assert(count <= INT_MAX);
	*strp = buf;
	return count;
}

