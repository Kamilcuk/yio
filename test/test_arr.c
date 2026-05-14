#include <yio_test.h>

static int my_printer(yio_printctx_t *t, const void *pnt, size_t elemsize) {
	(void)elemsize;
	return yio_printctx_printf(t, "{}", *(const int *)pnt);
}

int main() {
	const int arr[] = {1, 2, 3};

	// Test yio_arr(arr)
	YIO_TEST("1, 2, 3", "{}", yio_arr(arr));

	// Test yio_arr(arr, count)
	YIO_TEST("1, 2", "{}", yio_arr(arr, 2));

	// Test yio_arr(arr, count, separator)
	YIO_TEST("1 | 2 | 3", "{}", yio_arr(arr, 3, " | "));

	// Test yio_arr(arr, count, separator)
	YIO_TEST("1 | 2", "{}", yio_arr(arr, 2, " | "));

	// Test yio_arr(arr, count, separator, printer)
	YIO_TEST("1 | 2", "{}", yio_arr(arr, 2, " | ", my_printer));

	return 0;
}
