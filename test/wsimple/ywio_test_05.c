#include <ywio.h>
#include <stdlib.h>
int main() {
	wchar_t *str = ywformat(L"Hello", ywpwchar(' '), L"world", 5);
	str = yrewformat(str, L", have a day for ", 5, L" stars!\n");
	wprintf(L"%ls", str);
	free(str);
}
// PASS_REGULAR_EXPRESSION Hello world5, have a day for 5 stars!
