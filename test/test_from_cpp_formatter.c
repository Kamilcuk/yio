#include <yio_test.h>
#include <math.h>
#include <float.h>

int main() {
	char c = 120;

	YIO_TEST_EQ("    42", "{:6}", 42);
	YIO_TEST_EQ("x     ", "{:6}", (char)'x');
	YIO_TEST_EQ("x*****", "{:*<6}", (char)'x');
	YIO_TEST_EQ("x*****", "{:*<6}", (char)'x');
	YIO_TEST_EQ("*****x", "{:*>6}", (char)'x');
	YIO_TEST_EQ("**x***", "{:*^6}", (char)'x');
	YIO_TEST_EQ("   120", "{:6d}", c);
	YIO_TEST_EQ("true  ", "{:6}", (_Bool)true);

	YIO_TEST_EQ("1,+1,1, 1", "{0:},{0:+},{0:-},{0: }", 1);
	YIO_TEST_EQ("-1,-1,-1,-1", "{0:},{0:+},{0:-},{0: }", -1);

	YIO_TEST_EQ("+00120", "{:+06d}", c);
	YIO_TEST_EQ("0x000a", "{:#06x}", 0xa);
	YIO_TEST_EQ("-42   ", "{:<06}", -42);

#if YIO_HAS_FLOATf
	YIO_TEST_EQ("inf,+inf,inf, inf", "{0:},{0:+},{0:-},{0: }", INFINITY);
	YIO_TEST_EQ("nan,+nan,nan, nan", "{0:},{0:+},{0:-},{0: }", NAN);
	float pi = 3.14f;
	YIO_TEST_EQ("  3.140000", "{:10f}", pi);
	YIO_TEST_EQ("  3.140000", "{:{}f}", pi, 10);
	YIO_TEST_EQ("3.14000", "{:.5f}", pi);
	YIO_TEST_EQ("3.14000", "{:.{}f}", pi, 5);
	YIO_TEST_EQ("   3.14000", "{:10.5f}", pi);
	YIO_TEST_EQ("   3.14000", "{:{}.{}f}", pi, 10, 5);

	YIO_TEST_FAIL("{:{}f}", pi, 10.0);     // throws: width is not of integral type
	YIO_TEST_FAIL("{:{}f}", pi, -10);      // throws: width is negative
#endif
										   //
#if YIO_HAS_UNISTRING
	YIO_TEST_EQ(".🐱..", "{:.^5s}",   "🐱");
	YIO_TEST_EQ("🐱🐱",  "{:.8s}",    "🐱🐱🐱");
	YIO_TEST_EQ("🐱🐱.", "{:.<5.8s}", "🐱🐱🐱");
#endif
}
