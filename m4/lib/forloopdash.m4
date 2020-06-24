
/**
 * @ingroup m4
 * For each number between @b start_range to @b end_range
 * define @b identifier to dash joined with the number
 * and place @b replacement string joined with the separator.
 */
#define m4_forloopdash
m4_define_function(`m4_forloopdash~,
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
)m4_dnl;

/**
 * @ingroup m4
 * Internal function for m4_forloopdash
 */
#define _m4_forloopdash
m4_define(`_m4_forloopdash~,
	`$3`_$1~$4`~m4_ifelse(
		`$1~,
		`$2~,
		`~,
		`$5$0($6(`$1~),`$2~,`$3~,`$4~,`$5~,`$6~)~m4_dnl
)~m4_dnl
)m4_dnl;


/**
 * @brief Substitute for count times the replacement with leading dash.
 * @ingroup m4
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
#define m4_forloopdashX
m4_define_function(`m4_forloopdashX~,`m4_forloopdash(`X~,`$1~,`$2~,`$3~,`$4~)~)m4_dnl;

/**
 * @ingroup m4
 * @brief @see m4_forloopdashX
 */
#define m4_forloopdashI
m4_define_function(`m4_forloopdashI~,`m4_forloopdash(`I~,`$1~,`$2~,`$3~,`$4~)~)m4_dnl;

/**
 * @ingroup m4
 * @brief @see m4_forloopdashX
 */
#define m4_forloopdashY
m4_define_function(`m4_forloopdashY~,`m4_forloopdash(`Y~,`$1~,`$2~,`$3~,`$4~)~)m4_dnl;

