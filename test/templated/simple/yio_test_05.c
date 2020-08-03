#include <yπio.h>
#include <stdlib.h>
int main() {
	Ychar *str = yπformat("Hello", (char)' ', "world", 5);
	if (str == NULL) abort();
	str = yπreformat(str, ", have a day for ", 5, " stars!\n");
	if (str == NULL) abort();
	yπprint(str);
	yπprint(str);
	free(str);
}
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
