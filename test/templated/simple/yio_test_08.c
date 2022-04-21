#include <yπio.h>
#include <stdlib.h>
int main() {
	yπprintf(TC("Paded {:%>20}\n"), TC("word"));
}
// PASS_REGULAR_EXPRESSION Paded %%%%%%%%%%%%%%%%word
