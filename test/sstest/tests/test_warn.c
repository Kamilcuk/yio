#include <sstest.h>
int main() {
	SSTEST_WARN(1 == 1);
	SSTEST_WARN(1 == 0);
	SSTEST(1 == 2);
}
