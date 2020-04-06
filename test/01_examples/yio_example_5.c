// https://fresh2refresh.com/c-programming/c-printf-and-scanf/
#include <yio.h>
int main() {
    yprint("How old are you?\n");
    int a;
    if (yscan(&a).error != 0) abort();
    yprint("You are ", a, " years old.\n");
}
// INPUT 25
// PASS_REGULAR_EXPRESSION How old are you?
// PASS_REGULAR_EXPRESSION You are 25 years old\.
