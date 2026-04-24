#define YIO_USE_NONGNU_MACROS  1
#include <yio.h>
int main() {
	yio_print(1);
	yio_print(1, 2);
	yio_printf("{}", 1);
	yio_printf("{} {}", 1, 2);
}

