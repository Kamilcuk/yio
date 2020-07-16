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

int _yIO_astrftime_nonzero(char **strp, const size_t initialsize, const char *fmt, const struct tm *tm) {
	size_t bufsize = initialsize;
	char *buf = malloc(bufsize);
	if (buf == NULL) return -1;

	int count = 0;
	while ((count = strftime(buf, bufsize, fmt, tm)) == 0) {
		// Golden ratio resize
		assert(SIZE_MAX / 25 < bufsize);
		bufsize *= 25;
		bufsize /= 16;

		void *p = realloc(buf, bufsize);
		if (p == NULL) {
			free(buf);
			return -1;
		}
		buf = p;
	}

	// This is pretty safe and I'll do it, I'll trim unused memory
	void *p = realloc(buf, count);
	if (p == NULL) {
		free(buf);
		return -1;
	}
	buf = p;

	*strp = buf;
	return count;
}

