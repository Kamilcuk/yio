/**
 * @file
 * @date 2020-lip-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_allochelp.h"
#include "private.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef _yIO_HAS_MONETARY_H
#error
#endif
#if _yIO_HAS_MONETARY_H
#include <monetary.h>
#endif

int _yIO_astrftime_nonzero(char **strp, size_t bufsize, const char *fmt, const struct tm *tm) {
	char *initbuf = *strp;
	char *buf = initbuf;
	size_t count;
	while (1) {
		errno = 0;
		count = strftime(buf, bufsize, fmt, tm);
		if (count != 0) {
			break;
		} else if (errno != 0) {
			free(buf);
			return _yIO_ERROR(YIO_ERROR_STRFTIME, "strftime returned -1 and errno is set");
		}

		// Golden ratio resize
		assert((size_t)SIZE_MAX / 25u > bufsize);
		bufsize *= 25;
		bufsize /= 16;

		if (buf == initbuf) {
			buf = NULL;
		}

		void *p = realloc(buf, bufsize);
		if (p == NULL) {
			free(buf);
			return _yIO_ERROR(YIO_ERROR_STRFTIME_ENOMEM, "could not allocate memory for strftime");
		}
		buf = p;
	}
	//
	assert(count <= INT_MAX);
	*strp = buf;
	return count;
}

#if _yIO_HAS_MONETARY_H
int _yIO_astrfmon(char **strp, size_t bufsize, const char *fmt, double dd) {
	char *initbuf = *strp;
	char *buf = initbuf;
	ssize_t count;
	while (1) {
		errno = 0;
		count = strfmon(buf, bufsize, fmt, dd);
		if (count != -1) {
			break;
		} else if (errno != 0 && errno != E2BIG) {
			free(buf);
			return _yIO_ERROR(YIO_ERROR_STRFMON, "strfmon returned -1 and errno is set and it's not E2BIG");
		}

		// Golden ratio resize
		assert((size_t)SIZE_MAX / 25u > bufsize);
		bufsize *= 25;
		bufsize /= 16;

		if (buf == initbuf) {
			buf = NULL;
		}

		void *p = realloc(buf, bufsize);
		if (p == NULL) {
			free(buf);
			return _yIO_ERROR(YIO_ERROR_STRFTMON_ENOMEM, "could not allocate memory for strfmon");
		}
		buf = p;
	}
	//
	assert(count <= INT_MAX);
	*strp = buf;
	return count;
}
#endif

