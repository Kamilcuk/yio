
/**
 * @def m4_define_name(name, value)
 * @param name Macro name
 * @param value Expansion value.
 * @brief Defines macro to replace preserving arguments.
 *
 * Defines _name_ as a only name macro, such that
 * _name_ will expand to value and preserve all arguments
 *
 * @note Leading spaces between values are removed by m4.
 * Not that's is an issue, but it happens.
 */
#define m4_define_name(...)
m4_define(`m4_define_name~, `m4_ifelse(`$#~, `0~, ``$0~~,
	`_$0(`$1~, `$2~, `$~`#~, `$~`@~)~)~)
m4_define(`_m4_define_name~, `m4_define(`$1~,
	``$2~m4_ifelse(`$3~, `0~, `~, `($4)~)~)~)

m4_TEST(`m4_define_name(`nameA~, `valueA~)nameA~)
// PASS_REGULAR_EXPRESSION valueA
m4_TEST(`m4_define_name(`nameB~, `valueB~)nameB(1,2,3)~)
// PASS_REGULAR_EXPRESSION valueB\(1,2,3\)
m4_TEST(`m4_define_name(`nameC~, `valueC~)nameC( 1 ,   2   ,  3   )~)
// PASS_REGULAR_EXPRESSION valueC\(1 ,2   ,3   \)
