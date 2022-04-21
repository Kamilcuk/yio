#include <yπio.h>
#include <stdio.h>
#include <yio_test.h>
#include <stdlib.h>
int main() {
	yπprintf(TC("{} {:s} and {} {:s}\n"), (bool)1, (bool)1, (bool)0, (bool)0);
	// print_time.c
	time_t t = time(NULL);
	yπprint(yπptgmtime(t), "\n");
	yπprint(yπptlocaltime(t), "\n");
	// io_file.c
	yπfprintf(stderr, TC("Hello world\n"));
}
