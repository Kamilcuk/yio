
`/**
 * @ingroup m4
 *
 * Apply macro on quoted list
 * Example:
 *    m4_define(`m4_func~, `>1=$1 2=$2 3=$3<
 *    ~)
 *    m4_applyforeachq(`m4_func~, ```a~, `b~, `c~~, ``d~, `e~, `f~~, ``g~, `h~, `i~~~)
 * would result in:
 *    >1=a 2=b 3=c<
 *    >1=d 2=e 3=f<
 *    >1=g 2=h 3=i<
 */
#define m4_applyforeachq(function, qouted_list, separator) ~

m4_define(`_m4_applyforeachq_arg1~, `$1~)
m4_define(`_m4_applyforeachq_cat~, `_m4_applyforeachq_arg1($@)(m4_shift($@))~)
m4_define(`_m4_applyforeachq_quote~, `m4_ifelse(`$#~, `0~, `~, ``$*~~)~)
m4_define(`_m4_applyforeachq_separator~, `m4_ifelse(m4_eval(`$# > 2~), `1~, `$1~, `~)~)
m4_define(`m4_applyforeachq~, `m4_ifelse(
		_$0_quote($2),
		`~,
		`~,
		`_$0_cat(
				`$1~,
				_$0_arg1($2))_$0_separator(
						`$3~,
						$2)`~$0(
								`$1~,
								`m4_shift($2)~,
								`$3~)~)~)

m4_TEST(`m4_define(`m4_func~, `>1=$1 2=$2 3=$3<~)m4_applyforeachq(`m4_func~, `
		a,
		d,
		g~)~)
// PASS_REGULAR_EXPRESSION >1=a 2= 3=<>1=d 2= 3=<>1=g 2= 3=<
m4_TEST(`m4_define(`m4_func~, `>1=$1 2=$2 3=$3<~)m4_applyforeachq(`m4_func~, `
		z,
		x,
		y~, ` % ~)~)
// PASS_REGULAR_EXPRESSION >1=z 2= 3=< % >1=x 2= 3=< % >1=y 2= 3=<
m4_TEST(`m4_define(`m4_func~, `>1=$1 2=$2 3=$3<~)m4_applyforeachq(`m4_func~, ``1~, `2~, `3~~, ` % ~)~)
// PASS_REGULAR_EXPRESSION >1=1 2= 3=< % >1=2 2= 3=< % >1=3 2= 3=<
m4_TEST(`m4_define(`m4_func~, `>1=$1 2=$2 3=$3<~)m4_dnl
m4_applyforeachq(`m4_func~, ```a~, `b~, `c~~, ``d~, `e~, `f~~, ``g~, `h~, `i~~~, ` % ~)~)
// PASS_REGULAR_EXPRESSION >1=a 2=b 3=c< % >1=d 2=e 3=f< % >1=g 2=h 3=i<
