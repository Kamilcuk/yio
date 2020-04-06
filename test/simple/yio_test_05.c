#include <yio.h>
#include <stdlib.h>
int main() {
	char *str = yformat("Hello", (char)' ', "world", 5);
	str = yreformat(str, ", have a day for ", 5, " stars!\n");
	printf("%s", str);
	free(str);
}
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
