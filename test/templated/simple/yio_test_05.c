#include <yπio.h>
#include <stdlib.h>
#if _yIO_TYPE_YUIO
#include <unistdio.h>
#endif
int main() {
	Ychar *str = yπformat("Hello", (char)' ', "world", 5);
	str = yπreformat(str, ", have a day for ", 5, " stars!\n");
	Yprintf("%"YPRI, str);
	free(str);
}
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
