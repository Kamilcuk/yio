
/**
 * @brief generates numbers with separated by a comm
 * @param start_number Starting number of range, inclusive
 * @param end_number Ending number of range, inclusive
 * @param variable Variable to substitute in replacement string
 * @param replacemetn The replacement string
 * @param separator The separator.
 */
#define m4_seqcomma
m4_define_function(`m4_seqcomma~,
	`m4_forloop(
		m4_ifelse(`$3~, `~, `_m4_sEqCoMmA_vArIaBlE~, `$3~),
		`$1~,
		`$2~,
		`m4_ifelse(`$4~, `~, `m4_ifelse(`$3~, `~, `_m4_sEqCoMmA_vArIaBlE~, `$3~)~, `$4~)~,
		`m4_ifelse(`$5~, `~, `,~, `$5~)~)~)
m4_TEST(`m4_seqcomma(1, 5)~)
// PASS_REGULAR_EXPRESSION 1,2,3,4,5

/**
 * @ingroup m4
 * generates numbers with a leading dash separated by a comm
 */
#define m4_seqdashcomma
m4_define_function(`m4_seqdashcomma~,
	`m4_forloopdash(
		m4_ifelse(`$3~, `~, `_m4_sEqCoMmA_vArIaBlE~, `$3~),
		`$1~,
		`$2~,
		`m4_ifelse(`$4~, `~, `m4_ifelse(`$3~, `~, `_m4_sEqCoMmA_vArIaBlE~, `$3~)~, `$4~)~,
		`m4_ifelse(`$5~, `~, `,~, `$5~)~)~)
m4_TEST(`m4_seqdashcomma(1, 5)~)
// PASS_REGULAR_EXPRESSION _1,_2,_3,_4,_5

/**
 * @ingroup m4
 * @see m4_seqcomma
 */
#define m4_seqcommaX
m4_define_function(`m4_seqcommaX~, `m4_seqcomma(`$1~, `$2~, `X~, `$3~, `$4~)~)

/**
 * @def m4_seqdashcommaX(start_number, end_number, replacement_with_X, separator)
 * @ingroup m4
 * @see m4_seqdashcomma
 */
#define m4_seqdashcommaX
m4_define(`m4_seqdashcommaX~, `m4_seqdashcomma(`$1~, `$2~, `X~, `$3~, `$4~)~)
m4_TEST(`m4_seqdashcommaX(1, 5, `hello`~X~)~)
// PASS_REGULAR_EXPRESSION hello_1,hello_2,hello_3,hello_4,hello_5

/**
 * @see m4_seqcomma
 */
#define m4_seqcommaY
m4_define(`m4_seqcommaY~, `m4_seqcomma(`$1~, `$2~, `Y~, `$3~, `$4~)~)

/**
 * @ingroup m4
 * @see m4_seqdashcomma
 */
#define m4_seqdashcommaY
m4_define_function(`m4_seqdashcommaY~, `m4_seqdashcomma(`$1~, `$2~, `Y~, `$3~, `$4~)~)

/**
 * @see m4_seqcomma
 */
#define m4_seqcommaI
m4_define_function(`m4_seqcommaI~, `m4_seqcomma(`$1~, `$2~, `I~, `$3~, `$4~)~)

/**
 * @see m4_seqdashcomma
 */
#define m4_seqdashcommaI
m4_define_function(`m4_seqdashcommaI~, `m4_seqdashcomma(`$1~, `$2~, `I~, `$3~, `$4~)~)
