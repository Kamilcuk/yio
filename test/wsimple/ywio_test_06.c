#include <ywio.h>
#include <stdlib.h>
int main() {
	ywprint(L"Paded ", ywpcfmt("%20s", L"word"), L"\n");
}
// PASS_REGULAR_EXPRESSION Padded                word
