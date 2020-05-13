/**
 * @file ctx.c
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config();
#include "private.h"
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

/* printctx ---------------------------------------------------- */

int yio_printctx_out(yio_printctx_t *t, const Ychar *ptr, size_t size) {
	assert(t != NULL);
	assert(t->out != NULL);
	assert(ptr != NULL);
	const int ret = t->out(t->outarg, ptr, size);
	if (ret) return ret;
	assert(SIZE_MAX - size > t->writtencnt);
	t->writtencnt += size;
	return 0;
}

int yio_printctx_next(yio_printctx_t *t) {
	assert(t != NULL);
	assert(t->ifunc != NULL);
	assert(*t->ifunc != NULL);
	++t->ifunc;
	assert(*t->ifunc != NULL);
	return (*t->ifunc)(t);
}

struct yio_printfmt_s *yio_printctx_get_fmt(yio_printctx_t *t) {
	return &t->pf;
}

va_list *_yIO_printctx_inc_va_list(yio_printctx_t *t, size_t sizeof_realtype) {
	return _yIO_commonctx_inc_va_list(&t->c, sizeof_realtype);
}

int _yIO_printctx_print(yio_printctx_t *t, yio_printdata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yvbprintf(t->out, t->outarg, data, &va);
	va_end(va);
	if (ret < 0) {
		return ret;
	}
	assert(t->writtencnt < (size_t)INT_MAX - ret);
	t->writtencnt += ret;
	return 0;
}

/* scanctx ---------------------------------------------------- */

int yio_scanctx_in(yio_scanctx_t *t, int *c) {
	assert(t != NULL);
	if (t->unin) {
		t->unin = false;
		*c = t->ch;
		// _yIO_dbgln("in back '%s'", _yIO_printc(t->ch));
		return 0;
	}
	const int ret = t->in(t->inarg, &t->ch);
	*c = t->ch;
	// _yIO_dbgln("in '%s' %zu ret=%d", _yIO_printc(t->ch), t->scannedcnt, ret);
	if (ret) return ret;
	assert(SIZE_MAX - 1 > t->scannedcnt);
	++t->scannedcnt;
	return 0;
}

void yio_scanctx_unin(yio_scanctx_t *t) {
	assert(t->unin == false);
	t->unin = true;
	// _yIO_dbgln("unin '%s'", _yIO_printc(t->ch));
}

int yio_scanctx_next(yio_scanctx_t *t) {
	assert(t != NULL);
	assert(t->ifunc != NULL);
	assert(*t->ifunc != NULL);
	++t->ifunc;
	assert(*t->ifunc != NULL);
	return (*t->ifunc)(t);
}

struct yio_scanfmt_s *yio_scanctx_get_fmt(yio_scanctx_t *t) {
	return &t->sf;
}

size_t yio_scanctx_arg_size(yio_scanctx_t *t) {
	assert(t != NULL);
	assert(t->data != NULL);
	assert(t->data->argpntsizes != NULL);
	return t->data->argpntsizes[t->c.argsizespnt - t->data->argsizes - 1];
}

va_list *_yIO_scanctx_inc_va_list(yio_scanctx_t *t, size_t sizeof_realtype) {
	return _yIO_commonctx_inc_va_list(&t->c, sizeof_realtype);
}

int _yIO_scanctx_scan(yio_scanctx_t *t, yio_scandata_t *data, ...) {
	assert(t != NULL);
	va_list va;
	va_start(va, data);
	struct yio_scanret_s ret = yvbscanf(t->in, t->inarg, data, &va);
	va_end(va);
	assert(t->scannedcnt < (size_t)INT_MAX - ret.count);
	t->scannedcnt += ret.count;
	if (ret.lastchar != EOF) {
		t->unin = true;
		t->ch = ret.lastchar;
	}
	return ret.error;
}



