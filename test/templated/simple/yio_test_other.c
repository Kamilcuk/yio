#include <yio.h>
#include <stdio.h>
#include <yio_test.h>
#include <stdlib.h>
int main() {
	yio_printf("{} {:s} and {} {:s}\n", (bool)1, (bool)1, (bool)0, (bool)0);
	// io_file.c
	yio_fprintf(stderr, "Hello world\n");
}
