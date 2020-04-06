/*
 * yio_testcustom_type.c
 *
 *  Created on: 5 kwi 2020
 *      Author: kamil
 */
#include <yio.h>
#include <stdlib.h>

struct A {
	int a;
	int b;
};

// The yio printing callback.
static
int _yprint_A(yio_printctx_t *t) {
	struct A a = yio_printctx_va_arg(t, struct A);
	return yio_printctx_print(t, a.a, " ", a.b);
}

// The scanning callback.
static
int _yscan_A(yio_scanctx_t *t) {
	struct A *a = yio_scanctx_va_arg(t, struct A *);
	return yio_scanctx_scan(t, &a->a, " ", &a->b);
}

// Custom yio callbacks to be used with yio invokations.
// For type safety, make sure your arguments are proper type with `_Generic`.
#define yprint_A(var)  yiocb( _yprint_A, _Generic((var), struct A: (var), const struct A: (var)) )
#define yscan_A(var)   yiocb(  _yscan_A, _Generic((var), struct A *:(var)) )

int main() {
	struct A var_a = {
			1000,
			2000,
	};
	char *str = yformat(yprint_A(var_a), "\n");
	if (str == NULL) abort();
	struct A var_b;
	if (ysscan(str, yscan_A(&var_b)).error != 0) {
		yfprint(stderr, "ERROR: scanning error\n");
		exit(EXIT_FAILURE);
	}
	yprint(var_b.a, " ", var_b.b, "\n");
	free(str);
}
// PASS_REGULAR_EXPRESSION 1000 2000


