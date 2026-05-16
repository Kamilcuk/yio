#include <yio.h>
// PASS_REGULAR_EXPRESSION "line1\nline2"
// PASS_REGULAR_EXPRESSION "tab\tseparated"
// PASS_REGULAR_EXPRESSION "quote\"inside"
// PASS_REGULAR_EXPRESSION "slash\\\\back"
// PASS_REGULAR_EXPRESSION slash\\back
// PASS_REGULAR_EXPRESSION "\\(\\a\\+b\\)"
int main() {
    yio_print("line1\nline2\n");
    yio_print("tab\tseparated\n");
    yio_print("quote\"inside\n");
    yio_print("slash\\back\n");
    yio_print("slash\\back\n");
    yio_print("(a+b)\n");
    return 0;
}
