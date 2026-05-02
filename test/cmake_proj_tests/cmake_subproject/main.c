#include <yio.h>
int main() {
	const char *a = "Kamil";
	yio_stream("Hello world\n");
	yio_stream("My name is ", a, "!\n");
}

