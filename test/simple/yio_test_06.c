#include <yio.h>
#include <stdlib.h>
int main() {
	yprint("Paded ", yppfmt("{:%>20}"), "word", " suffix\n");
}
// PASS_REGULAR_EXPRESSION Paded %%%%%%%%%%%%%%%%word suffix
