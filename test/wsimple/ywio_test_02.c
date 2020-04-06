#include <ywio.h>
int main() {
	int arg = 1;
	ywprint(arg);
	long larg = -2;
	size_t hello = 44;
	const wchar_t str[] = L"Hello world";
	ywprint(L" ", arg, L" ", larg, L" ", hello, L" ", str, L"\n");
}
// PASS_REGULAR_EXPRESSION 1 -2 44 Hello world
