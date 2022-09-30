/*
 * yio_testcustom_type.c
 *
 *  Created on: 5 kwi 2020
 *      Author: kamil
 */

#include <yio.h>
#include <stdlib.h>

/// A custom type
struct A {
	int a;
	int b;
};

// The yio printing callback.
static int yio_print_A(yio_printctx_t *t) {
	// The callback *has to* call yio_printctx_va_arg *first*.
	struct A a = yio_printctx_va_arg(t, struct A);
	// Then the call *has to* check yio_printctx_init, and if returns non-zero, return the value.
	int ret = yio_printctx_init(t);
	if (ret) return ret;
	// Then you can actually print the resulting value.
	return yio_printctx_printf(t, "{} {}", a.a, a.b);
}

// Add both const and non-const of the type to callback function.
YIO_ADD_TYPE(struct A, yio_print_A)
#include YIO_ADD_TYPE_INC()
YIO_ADD_TYPE(const struct A, yio_print_A)
#include YIO_ADD_TYPE_INC()

// Alternative way is to create a custom format callback.
// Custom yio callbacks to be used with yio invokations.
// For type safety, make sure your arguments are proper type with _Generic.
#define yprint_A(var)  \
		yiocb( yio_print_A, ( (void)_Generic((var), struct A: 1, const struct A: 1), (var)) )
//                                  ^^^^^^^^ - protect against invalid arguments
//             ^^^^^^^^^^^ - the callback to call

int main() {
	struct A var_a = {
			1000,
			2000,
	};

	// The custom printing modifier is applied "on top" the argument.
	// It calls the function given in the callback.
	char *str = yformatf("{}\n", yprint_A(var_a));
	yprintf("{}\n", str);

	// This uses the PRINT_SLOT_100 above.
	str = yreformatf(str, "{}\n", var_a);
	yprintf("{}\n", str);

	if (str == NULL) abort();
	free(str);
}
// PASS_REGULAR_EXPRESSION 1000 2000
// PASS_REGULAR_EXPRESSION 1000 2000


