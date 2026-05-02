#include <yio.h>
#include <stdlib.h>
int main() {
	char *str = NULL;
	if (yio_asstream(&str, "Hello", (char)' ', "world", 5) < 0) abort();
	if (yio_appendstream(&str, ", have a day for ", 5, " stars!\n") < 0) abort();
	yio_stream(str);
	yio_stream(str);
	free(str);
}
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
