
/**
 * m4_forloopdash(identifier, start_range, end_range, replacement, separator)
 * For each number between `start_range` to `end_range`
 * define `identifier` to dash joined with the number
 * and place `replacement` string joined with the separator.
 */
m4_define(`m4_forloopdash~,
	`m4_ifelse(
		m4_eval(`($2) <= ($3)~),
		`1~,
		`m4_pushdef(`$1~)_m4_forloopdash(
			m4_eval(`$2~),
			m4_eval(`$3~),
			`m4_define(`$1~,~,
			`)$4~,
			`$5~,
			`m4_incr~)m4_popdef(`$1~)~,
		`m4_ifelse(
			m4_eval(`($2) >= ($3)~),
			`1~,
			`m4_pushdef(`$1~)_m4_forloopdash(
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
 * _m4_forloopdash(iterator, end, define_new_ierator, body_new_iterator, incr/decr)
 * Internal function for m4_forloopdash
 */
m4_define(`_m4_forloopdash~,
	`$3`_$1~$4`~m4_ifelse(
		`$1~,
		`$2~,
		`~,
		`$5$0($6(`$1~),`$2~,`$3~,`$4~,`$5~,`$6~)~m4_dnl
)~m4_dnl
)


/**
 * @def m4_forloopdashX(start_range, end_range, substitution_with_X, separator)
 * @brief Substitute for count times the replacement with leading dash.
 *
 * Replace X in substitution_with_X for the number with added leading dash
 * and join the replacement with the separator
 * for each number within (start_range, end_range).
 *
 * Example:
 *     m4_forloopdashX(1, 5, ``hello~X~, `, ~)
 * results in:
 *     hello_1, hello_2, hello_3, hello_4, hello_5
 * Example:
 *     m4_forloopdashX(1, 5, ``hello~X~)
 * results in:
 *     hello_1hello_2hello_3hello_4hello_5
 */
m4_define(`m4_forloopdashX~,`m4_forloopdash(`X~,`$1~,`$2~,`$3~,`$4~)~)

/**
 * @def m4_forloopdashI(start_range, end_range, substitution_with_I, separator)
 * @brief @see m4_forloopdashX
 */
m4_define(`m4_forloopdashI~,`m4_forloopdash(`I~,`$1~,`$2~,`$3~,`$4~)~)

/**
 * @def m4_forloopdashY(start_range, end_range, substitution_with_I, separator)
 * @brief @see m4_forloopdashX
 */
m4_define(`m4_forloopdashY~,`m4_forloopdash(`Y~,`$1~,`$2~,`$3~,`$4~)~)

