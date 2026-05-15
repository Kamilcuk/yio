#include <yio.h>
#include <stdlib.h>
int main() {
	yio_print("Paded {:%>20}\n", "word");
}
// PASS_REGULAR_EXPRESSION Paded %%%%%%%%%%%%%%%%word
