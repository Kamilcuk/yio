#include "yio.h"
#include <iostream>

int main() {
	int err = yio_printf("Hello from C++! {} {}\n", 42, 3.14);
	if (err < 0) {
		std::cerr << "yio_printf failed with " << err << std::endl;
		return 1;
	}
	return 0;
}
