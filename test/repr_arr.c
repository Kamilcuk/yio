#include <yio_test.h>

int main() {
	const int arr[] = {1, 2, 3};
	yio_printf("{}\n", yio_arr(arr));
}
// PASS_REGULAR_EXPRESSION: 1, 2, 3
