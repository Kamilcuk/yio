#include <yio.h>
int main() {
	yio_stream("Paded ", yio_pfmt("{:>20s}"), "word", "\n");
}
// PASS_REGULAR_EXPRESSION Paded                 word
