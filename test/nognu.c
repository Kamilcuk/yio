#define YIO_USE_NONGNU_MACROS  1
#include <yio.h>
int main() {
	yio_stream(1);
	yio_stream(1, 2);
	yio_print("{}", 1);
	yio_print("{} {}", 1, 2);
}

