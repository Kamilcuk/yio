#include <yio.h>
#include <yio_test.h>
#include <stdlib.h>

int main() {
	char *s = NULL;

	yio_asprintln(&s, "hello");
	YIO_TEST_EQ(s, "hello\n");
	free(s); s = NULL;

	yio_asprintln(&s, "{}", 123);
	YIO_TEST_EQ(s, "123\n");
	free(s); s = NULL;

	yio_asstreamln(&s, "stream");
	YIO_TEST_EQ(s, "stream\n");
	free(s); s = NULL;

	yio_asstreamln(&s, 456);
	YIO_TEST_EQ(s, "456\n");
	free(s); s = NULL;

#if YIO_HAS_FSTRING
	// Testing f-strings if supported
	yio_print("Testing f-strings\n");
	int a = 1;
	yio_asprintln_f(&s, "{a}");
	YIO_TEST_EQ(s, "1\n");
	free(s); s = NULL;
#endif

	return 0;
}
