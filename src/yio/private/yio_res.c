/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_res.h"
#include "../yio_error.h"
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

void YYIO_res_init(YYIO_res *t, char *bufbegin, char *bufend) {
	assert((bufbegin != NULL && bufend != NULL) || (bufbegin == NULL && bufend == NULL));
	YYIO_res ret;
	ret.beg = bufbegin;
	ret.pos = ret.beg;
	ret.end = bufend;
	ret.is_dynamic = bufbegin == NULL;
	*t = ret;
}

int YYIO_res_putsn(YYIO_res *t, const char *ptr, size_t size) {
	while (YYIO_res_free_size(t) < size) {
		const int err = YYIO_res_reserve_more(t);
		if (err) return err;
	}
	memcpy(t->pos, ptr, size * sizeof(*t->pos));
	t->pos += size;
	return 0;
}

static
int YYIO_res_yprintf_cb(void *ptr, const char *data, size_t count) {
	YYIO_res *o = ptr;
	while (count--) {
		const int err = YYIO_res_putc(o, *data++);
		if (err) return err;
	}
	return 0;
}

int YYIO_res_yprintf_in(YYIO_res *t, yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int err = yvbprintf(YYIO_res_yprintf_cb, t, data, fmt, &va);
	va_end(va);
	if (err < 0) {
		return err;
	}
	return 0;
}

bool YYIO_res_remove_trailing_zeros_and_comma(YYIO_res *t) {
	bool fractional_part_removed = false;
	// remove trailing zeros
	char * const dest = t->pos;
	// god help me
	char *p = dest - 1;
	// there is dot, so the following loop will always stop
	while (*p == '0' && p != t->beg) {
		--p;
	}
	assert(isdigit((unsigned char)*p) || *p == '.' || strchr("abcdefABCDEF", *p));
	if (*p != '.') {
		++p;
	} else {
		fractional_part_removed = true;
	}
	t->pos = p;
	assert(t->pos >= t->beg);
	assert(t->end >= t->pos);
	return fractional_part_removed;
}
