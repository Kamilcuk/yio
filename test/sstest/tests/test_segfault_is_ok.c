#include <sstest.h>
int main() {
	SSTEST(1);
	SSTEST(*(volatile int*)0 = 1);
}
