#include <yio.h>
#include <stdlib.h>
int main() {
	char *str = NULL;
	if (yio_asprint(&str, "Hello", (char)' ', "world", 5) < 0) abort();
	if (yio_append_s(&str, ", have a day for ", 5, " stars!\n") < 0) abort();
	yio_print(str);
	yio_print(str);
	free(str);
}
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
