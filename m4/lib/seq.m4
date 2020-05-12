
/**
 * @def m4_seqcomma(start_number, end_numbers [, variable, replacement [, separator]])
 * @brief generates numbers with separated by a comm
 * @param start_number Starting number of range, inclusive
 * @param end_number Ending number of range, inclusive
 * @param variable Variable to substitute in replacement string
 * @param replacemetn The replacement string
 * @param separator The separator.
 *
 * example:
 *    m4_seqcomma(1, 5)
 * outputs:
 *   1,2,3,4,5
 */
m4_define(`m4_seqcomma~, 
	`m4_forloop(
		m4_ifelse(`$3~, `~, `_m4_sEqCoMmA_vArIaBlE~, `$3~),
		`$1~,
		`$2~,
		`m4_ifelse(`$4~, `~, `m4_ifelse(`$3~, `~, `_m4_sEqCoMmA_vArIaBlE~, `$3~)~, `$4~)~,
		`m4_ifelse(`$5~, `~, `,~, `$5~)~)~)

/**
 * m4_seqdashcomma(start_number, end_numbers)
 * generates numbers with a leading dash separated by a comm
 * example:
 *   m4_seqdashcomma(1, 5)
 * outputs:
 *   _1,_2,_3,_4,_5
 */
m4_define(`m4_seqdashcomma~,
	`m4_forloopdash(
		m4_ifelse(`$3~, `~, `_m4_sEqCoMmA_vArIaBlE~, `$3~),
		`$1~,
		`$2~,
		`m4_ifelse(`$4~, `~, `m4_ifelse(`$3~, `~, `_m4_sEqCoMmA_vArIaBlE~, `$3~)~, `$4~)~,
		`m4_ifelse(`$5~, `~, `,~, `$5~)~)~)

/**
 * @def m4_seqcommaX(start_number, end_number, replacement_with_X, separator)
 * @see m4_seqcomma
 */
m4_define(`m4_seqcommaX~, `m4_seqcomma(`$1~, `$2~, `X~, `$3~, `$4~)~)

/**
 * @def m4_seqdashcommaX(start_number, end_number, replacement_with_X, separator)
 * @see m4_seqdashcomma
 */
m4_define(`m4_seqdashcommaX~, `m4_seqdashcomma(`$1~, `$2~, `X~, `$3~, `$4~)~)

/**
 * @def m4_seqcommaY(start_number, end_number, replacement_with_X, separator)
 * @see m4_seqcomma
 */
m4_define(`m4_seqcommaY~, `m4_seqcomma(`$1~, `$2~, `Y~, `$3~, `$4~)~)

/**
 * @def m4_seqdashcommaY(start_number, end_number, replacement_with_X, separator)
 * @see m4_seqdashcomma
 */
m4_define(`m4_seqdashcommaY~, `m4_seqdashcomma(`$1~, `$2~, `Y~, `$3~, `$4~)~)
/**
 * @def m4_seqcommaI(start_number, end_number, replacement_with_X, separator)
 * @see m4_seqcomma
 */
m4_define(`m4_seqcommaI~, `m4_seqcomma(`$1~, `$2~, `I~, `$3~, `$4~)~)

/**
 * @def m4_seqdashcommaI(start_number, end_number, replacement_with_X, separator)
 * @see m4_seqdashcomma
 */
m4_define(`m4_seqdashcommaI~, `m4_seqdashcomma(`$1~, `$2~, `I~, `$3~, `$4~)~)
