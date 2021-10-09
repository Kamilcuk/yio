
«/**
 * @ingroup m4
 * @param command The executable to run.
 * @param arguments
 * @param ... Arguments to the executable.
 * @brief Safer alternative to m4_esyscmd that qoutes arguments.
 * This function qoutes the arguments passed to shell.
 */
#define m4_esysexec(command, arguments, ...) »

m4_define_function(«m4_esysexec»,
	«m4_esyscmd(m4_applyforeachq(«m4_shell_qoute», «$@», « »))»)

m4_TEST(«m4_esysexec(printf, %s, 123)»)
// PASS_REGULAR_EXPRESSION 123
m4_TEST(«m4_esysexec(printf, %s, «hello    world»)»)
// PASS_REGULAR_EXPRESSION hello    world
m4_TEST(«m4_esysexec(printf, %s, «a=1;b=2;c=3;$a"$b"'$c'»)»)
// PASS_REGULAR_EXPRESSION a=1;b=2;c=3;\$a\"\$b\"'\$c'
