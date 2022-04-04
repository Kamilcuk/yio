/*
 * yio_testcustom_type.c
 *
 *  Created on: 5 kwi 2020
 *      Author: kamil
 */

// You could add this to for example your compiler defines.
// Remember about comma on the end.
// The slots are macro substituted and currently
// slots are generated from 100 to 199. So you can use any number in that region,
// so take a random number and use it.
#define YIO_PRINT_SLOT_100  struct A: _yprint_A, const struct A: _yprint_A,

// Remember, as with feature_test_macros(5), first macro, then includes
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

int main() {
	struct A var_a = {
			1000,
			2000,
	};
	char *str = yformat(var_a, "\n");
	if (str == NULL) exit(EXIT_FAILURE);
	free(str);
}
// PASS_REGULAR_EXPRESSION 1000 2000


