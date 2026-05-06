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

// Add the type to callback function.
YIO_ADD_TYPE(struct A, yio_print_A)
#include YIO_ADD_TYPE_INC()

// Alternative way is to create a custom format callback.
// Custom yio callbacks to be used with yio invokations.
// For type safety, make sure your arguments are proper type with _Generic.
#define yprint_A(var)  \
		yio_callback( yio_print_A, _Generic((var), struct A: (var)) )
//                             ^^^^^^^^ - protect against invalid arguments
//                ^^^^^^^^^^^ - the callback to call

int main() {
	struct A var_a = {
			1000,
			2000,
	};

	// The custom printing modifier is applied "on top" the argument.
	// It calls the function given in the callback.
	char *str = NULL;
	yio_asprintf(&str, "{}\n", yprint_A(var_a));
	yio_printf("{}\n", str);

	// This uses the PRINT_SLOT_100 above.
	yio_append(&str, "{}\n", var_a);
	yio_printf("{}\n", str);

	if (str == NULL) abort();
	free(str);
}
// PASS_REGULAR_EXPRESSION 1000 2000
// PASS_REGULAR_EXPRESSION 1000 2000


