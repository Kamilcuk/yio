#include <sstest.h>
int main() {
	SSTEST(1 == 1, "string");
	SSTEST(1 == 1, "string %s", "argument");
}

