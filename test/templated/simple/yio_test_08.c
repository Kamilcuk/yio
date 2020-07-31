#include <yπio.h>
#include <stdlib.h>
int main() {
	yπprintf(Yc("Paded {:%>20}\n"), Yc("word"));
}
// PASS_REGULAR_EXPRESSION Paded %%%%%%%%%%%%%%%%word
