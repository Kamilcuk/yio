
/**
 * @ingroup m4
 */
#define m4_applyforloop(...)
m4_define(`m4_applyforloop~, `m4_forloop(
	`_m4_applyforloop_iterator~,
	`$1~,
	`$2~, 
	`$3(_m4_applyforloop_iterator)~,
	`$4~)~)

m4_TEST(`m4_define(`func~, `m4_forloop(`J~, 1, $1, `@$1, J%~)~)m4_applyforloop(1, 4, `func~)~)
// PASS_REGULAR_EXPRESSION @1, 1%@2, 1%@2, 2%@3, 1%@3, 2%@3, 3%@4, 1%@4, 2%@4, 3%@4, 4%
