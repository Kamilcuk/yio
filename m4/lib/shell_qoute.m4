
/**
 * @brief Quote the argument according to the shell.
 */
#define m4_shell_qoute(argument)
m4_define(`m4_shell_qoute~,
		`'m4_patsubst(
			`$1~,
			`'~,
			`'\\''~)'~)
