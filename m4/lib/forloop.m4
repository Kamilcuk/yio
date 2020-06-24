
/**
 * @ingroup m4
 * Generates iterators from start_range to end_range substitution
 * the iterator in substitution string separating substitutiong
 * by the separator.
 */
#define m4_forloop(i, 1, 1)
m4_define_function(`m4_forloop~,
	`m4_ifelse(
		m4_eval(`($2) <= ($3)~),
		`1~,
		`m4_pushdef(`$1~)_m4_forloop(
			m4_eval(`$2~),
			m4_eval(`$3~),
			`m4_define(`$1~,~,
			`)$4~,
			`$5~,
			`m4_incr~)m4_popdef(`$1~)~,
		`m4_ifelse(
			m4_eval(`($2) >= ($3)~),
			`1~,
			`m4_pushdef(`$1~)_m4_forloop(
				m4_eval(`$2~),
				m4_eval(`$3~),
				`m4_define(`$1~,~,
				`)$4~,
				`$5~,
				`m4_decr~)m4_popdef(`$1~)~,
			`~)~)~)

/**
 * @ingroup m4
 * Internal function
 */
m4_define_function(`_m4_forloop~,
	`$3`$1~$4`~m4_ifelse(
		`$1~,
		`$2~,
		`~,
		`$5$0($6(`$1~),`$2~,`$3~,`$4~,`$5~,`$6~)~)~)

/**
 * @ingroup m4
 * @brief substitute for count times the replacement.
 *
 * Replace X in substitution_with_X and join with separator
 * for each number within (start_range, end_range)
 * Example:
 *     m4_forloopX(1, 5, ``hello_~X~, `, ~)
 * results in:
 *     hello_1, hello_2, hello_3, hello_4, hello_5
 * Example:
 *     m4_forloopX(1, 5, ``hello_~X~)
 * results in:
 *     hello_1hello_2hello_3hello_4hello_5
 */
#define m4_forloopX(1, 1, ...)
m4_define_function(`m4_forloopX~,`m4_forloop(`X~,`$1~,`$2~,`$3~,`$4~)~)

/**
 * @ingroup m4
 * @brief @see m4_forloopX
 */
#define m4_forloopI(1, 1, ...)
m4_define_function(`m4_forloopI~,`m4_forloop(`I~,`$1~,`$2~,`$3~,`$4~)~)

/**
 * @ingroup m4
 * @brief @see m4_forloopX
 */
#define m4_forloopY(1, 1, ...)
m4_define_function(`m4_forloopY~,`m4_forloop(`Y~,`$1~,`$2~,`$3~,`$4~)~)

