#include <sstest.h>

int func() {
	return 42;
}

int main() {
	// Basic usage - just an expression.
	// Will print "<file>:<line>:<func>: OK: <expr>"
	SSTEST(func() == 42);
	// Will print "<file:<line>:<func>: ERROR: <expr>"
	SSTEST(func() == 1);

	// Custom printf-ish message.
	// Will print "<file>:<line>:<func>: OK: <expr>
	// Custom message is only displayed on failure.
	SSTEST(func() == 42, "Custom message %d", func());
	// Will print "<file>:<line>:<func>: ERROR: <expr>: Custom message 42
	SSTEST(func() == 1, "Custom message %d", func());

	// WARNing
	SSTEST_WARN(1 == 0);
	SSTEST_WARN(1 == 0, "Custom message %d", func());

	// Immidately exit with abort() if the expression is false.
	SSTEST_ASSERT(1 == 1);

	// Extended version - internal API
	const int cache = func();
	SSTEST_EX((), "func() == 42", cache == 42, "Custom message %d", cache);
}
