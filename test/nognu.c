#define YIO_USE_NONGNU_MACROS  1
#include <yio.h>
int main() {
	yprint(1);
	yprint(1, 2);
	yprintf("{}", 1);
	yprintf("{} {}", 1, 2);
}

