
/**
 * @def m4_forloop(iterator, start_range, end_range, substitution, separator)
 * @ingroup m4
 * Generates iterators from start_range to end_range substitution
 * the iterator in substitution string separating substitutiong
 * by the separator.
 * Example:
 *    m4_forloop(`I~, 1, 4, `m4_forloop(`J~, 1, I, `(I, J)~)~)
 * will output:
 *    (1, 1)(2, 1)(2, 2)(3, 1)(3, 2)(3, 3)(4, 1)(4, 2)(4, 3)(4, 4)
 */
#define m4_forloop(i, 1, 1)
m4_define(`m4_forloop~,
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
			`~m4_dnl
)~m4_dnl
)~m4_dnl
)

/**
 * @def _m4_forloop(iterator, end, define_new_ierator, body_new_iterator, incr/decr)
 * Internal function for m4_forloop
 */
m4_define(`_m4_forloop~,
	`$3`$1~$4`~m4_ifelse(
		`$1~,
		`$2~,
		`~,
		`$5$0($6(`$1~),`$2~,`$3~,`$4~,`$5~,`$6~)~m4_dnl
)~m4_dnl
)

/**
 * @def m4_forloopX(start_range, end_range, substitution_with_X, separator)
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
m4_define(`m4_forloopX~,`m4_forloop(`X~,`$1~,`$2~,`$3~,`$4~)~)

/**
 * @def m4_forloopI(start_range, end_range, substitution_with_I, separator)
 * @brief @see m4_forloopX
 */
#define m4_forloopI(1, 1, ...)
m4_define(`m4_forloopI~,`m4_forloop(`I~,`$1~,`$2~,`$3~,`$4~)~)

/**
 * @def m4_forloopY(start_range, end_range, substitution_with_I, separator)
 * @brief @see m4_forloopX
 */
#define m4_forloopY(1, 1, ...)
m4_define(`m4_forloopY~,`m4_forloop(`Y~,`$1~,`$2~,`$3~,`$4~)~)

