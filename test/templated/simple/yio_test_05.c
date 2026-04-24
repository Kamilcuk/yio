#include <yio.h>
#include <stdlib.h>
int main() {
	char *str = yio_format("Hello", (char)' ', "world", 5);
	if (str == NULL) abort();
	str = yio_reformat(str, ", have a day for ", 5, " stars!\n");
	if (str == NULL) abort();
	yio_print(str);
	yio_print(str);
	free(str);
}
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
