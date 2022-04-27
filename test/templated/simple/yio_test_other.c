#include <yπio.h>
#include <stdio.h>
#include <yio_test.h>
#include <stdlib.h>
int main() {
	yπprintf(TC("{} {:s} and {} {:s}\n"), (bool)1, (bool)1, (bool)0, (bool)0);
	// io_file.c
	yπfprintf(stderr, TC("Hello world\n"));
}
