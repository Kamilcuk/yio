#include <yio.h>
int main() {
	yio_print("Paded ", yio_pfmt("{:%>20}"), "word", " suffix\n");
}
// PASS_REGULAR_EXPRESSION Paded %%%%%%%%%%%%%%%%word suffix
