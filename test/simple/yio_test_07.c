#include <yio.h>
int main() {
	yprint("Paded ", ypcfmt("%20s"), "word", "\n");
}
// PASS_REGULAR_EXPRESSION Paded                 word
