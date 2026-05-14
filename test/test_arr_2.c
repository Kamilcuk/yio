#include <yio.h>
#include <yio_test.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
	// 1. Array via pointer
	int arr_int[] = {10, 20, 30};
	int *ptr_int = arr_int;
	YIO_TEST_EQ("10, 20, 30", "{}", yio_arr(ptr_int, 3));

	// 2. Different types
	float arr_float[] = {1.1f, 2.2f};
	YIO_TEST_EQ("1.1, 2.2", "{:.1f}", yio_arr(arr_float));

	char arr_char[] = {'a', 'b', 'c'};
	YIO_TEST_EQ("a, b, c", "{}", yio_arr(arr_char));

	short arr_short[] = {1, 2, 3};
	YIO_TEST_EQ("1, 2, 3", "{}", yio_arr(arr_short));

	long long arr_ll[] = {10000000000LL, 20000000000LL};
	YIO_TEST_EQ("10000000000, 20000000000", "{}", yio_arr(arr_ll));

	bool arr_bool[] = {true, false, true};
	YIO_TEST_EQ("true, false, true", "{}", yio_arr(arr_bool));

	// 3. Dynamically allocated array via malloc
	int *arr_malloc = malloc(3 * sizeof(int));
	if (arr_malloc) {
		arr_malloc[0] = 100;
		arr_malloc[1] = 200;
		arr_malloc[2] = 300;
		YIO_TEST_EQ("100, 200, 300", "{}", yio_arr(arr_malloc, 3));
		free(arr_malloc);
	}

	return 0;
}
