#include <yio.h>
#include <stdlib.h>
int main() {
	char *str = yformat("Hello", (char)' ', "world", 5);
	if (str == NULL) abort();
	str = yreformat(str, ", have a day for ", 5, " stars!\n");
	if (str == NULL) abort();
	yprint(str);
	yprint(str);
	free(str);
}
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
