#include <yio_test.h>
#include <time.h>
#include <locale.h>

int main(void) {
	/* Basic positional usage */
	YIO_TEST("a\\0011", "{}", yio_repr_oct("a\0011"));
	YIO_TEST("a\\0011", "{0}", yio_repr_oct("a\0011"));

	/* yio_repr should count as exactly 1 positional argument */
	YIO_TEST("a\\0011 X", "{0} {1}", yio_repr_oct("a\0011"), "X");
	YIO_TEST("X a\\0011", "{1} {0}", yio_repr_oct("a\0011"), "X");

	/* yio_repr_caret multiple arguments */
	YIO_TEST("a^A Y", "{0} {1}", yio_repr_caret("a\001"), "Y");
	YIO_TEST("Y a^A Y", "{1} {0} {1}", yio_repr_caret("a\001"), "Y");

	/* Mixed repr arguments */
	YIO_TEST("a\\0011 a^A", "{0} {1}", yio_repr_oct("a\0011"), yio_repr_caret("a\001"));
	YIO_TEST("a^A a\\0011", "{1} {0}", yio_repr_oct("a\0011"), yio_repr_caret("a\001"));

	/* Dynamic formatting specifiers */
	YIO_TEST("a^A   Y", "{0:{2}} {1}", yio_repr_caret("a\001"), "Y", 5);
	YIO_TEST("Y a\\0011   Z", "{1} {0:{3}} {2}", yio_repr_oct("a\0011"), "Y", "Z", 8);

	/* 5+ arguments, testing more complex positioning and interaction */
	YIO_TEST(
		"start a\\0011 mid a^A end", 
		"{0} {1} {2} {3} {4}", 
		"start", yio_repr_oct("a\0011"), "mid", yio_repr_caret("a\001"), "end"
	);
	
	YIO_TEST(
		"end a^A mid a\\0011 start", 
		"{4} {3} {2} {1} {0}", 
		"start", yio_repr_oct("a\0011"), "mid", yio_repr_caret("a\001"), "end"
	);

	YIO_TEST(
		"a^A start a^A mid a\\0011 end", 
		"{3} {0} {3} {2} {1} {4}", 
		"start", yio_repr_oct("a\0011"), "mid", yio_repr_caret("a\001"), "end"
	);

	/* 5+ args with dynamic specifiers */
	YIO_TEST(
		"start a\\0011   mid a^A end", 
		"{0} {1:{5}} {2} {3} {4}", 
		"start", yio_repr_oct("a\0011"), "mid", yio_repr_caret("a\001"), "end", 8
	);

	YIO_TEST(
		"start a\\0011   mid a^A   end", 
		"{0} {1:{5}} {2} {3:{6}} {4}", 
		"start", yio_repr_oct("a\0011"), "mid", yio_repr_caret("a\001"), "end", 8, 5
	);

	/* Mixing yio_repr and struct tm */
	const struct tm t = {
		.tm_year = 112,
		.tm_mon  = 9,
		.tm_mday = 9,
		.tm_hour = 8,
		.tm_min  = 10,
		.tm_sec  = 20,
	};
	setlocale(LC_ALL, "C");

	YIO_TEST(
		"a\\0011 2012-10-09 a^A 08:10:20",
		"{0} {1:%Y-%m-%d} {2} {1:%H:%M:%S}",
		yio_repr_oct("a\0011"), t, yio_repr_caret("a\001")
	);

	YIO_TEST(
		"start a\\0011 mid 2012 end a^A 10",
		"{0} {1} {2} {3:.4%Y} {4} {5} {3:.2%m}",
		"start", yio_repr_oct("a\0011"), "mid", t, "end", yio_repr_caret("a\001")
	);

	/* dynamic format precision with struct tm and repr */
	YIO_TEST(
		"a\\0011   2012 a^A",
		"{0:{3}} {1:.{4}%Y-%m-%d} {2}",
		yio_repr_oct("a\0011"), t, yio_repr_caret("a\001"), 8, 4
	);

	return 0;
}
