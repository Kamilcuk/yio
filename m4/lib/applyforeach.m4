
/**
 * @def m4_applyforeach(macro, args, separator)
 * @ingroup m4
 * @param macro A macro name to apply arguments to
 * @param argslist A list og macro arguments in the form of
 * `((arg1, arg2, ...), (args1, arg2, ...), ...)~.
 * Remember to qoute it!!
 * @param separator An optional separator to separate elements.
 *
 * Apply macro on arguments in brackets.
 * The list of arguments is shifted and is applied on each argument.
 * Example:
 *     m4_define(`func~, `>1=$1 2=$2 3=$3<~) m4_applyforeach(`func~, `((`a~, `b~, `c~), (`d~, `e~, `f~), (`g~, `h~, `i~))~, ` % ~)
 * would output:
 *    >1=a 2=b 3=c< % >1=d 2=e 3=f< % >1=g 2=h 3=i<
 */
#define m4_applyforeach

m4_define(`_m4_applyforeach_arg1~, `$1~)
m4_define(`_m4_applyforeach_cat~, `$1$2~)
m4_define(`_m4_applyforeach_separator~, `m4_ifelse(`$2~, `()~, `~, `$1~)~)
m4_define(`m4_applyforeach~, `m4_ifelse(`$2~, `()~, `~,
    `_m4_applyforeach_cat(
    	`$1~,
    	_m4_applyforeach_arg1$2)_m4_applyforeach_separator(
    		`$3~,
    		(m4_shift$2))m4_applyforeach(
    			`$1~,
    			(m4_shift$2),
    			`$3~)~)~)

m4_TEST(`m4_define(`func~, `>1=$1 2=$2 3=$3<~)m4_applyforeach(`func~, `((`a~, `b~, `c~), (`d~, `e~, `f~), (`g~, `h~, `i~))~, ` % ~)~)
// PASS_REGULAR_EXPRESSION >1=a 2=b 3=c< % >1=d 2=e 3=f< % >1=g 2=h 3=i<
m4_TEST(`m4_define(`func~, `@$1<~)m4_applyforeach(`func~, `((bar), (foo))~, `:~)~)
// PASS_REGULAR_EXPRESSION @bar<:@foo<

/**
 * @def m4_applyforeachq(macro, ``args, ...~, `args, ...~, ...~)
 * @ingroup m4
 *
 * Apply macro on quoted list
 * Example:
 *    m4_define(`func~, `>1=$1 2=$2 3=$3<
 *    ~)
 *    m4_applyforeachq(`func~, ```a~, `b~, `c~~, ``d~, `e~, `f~~, ``g~, `h~, `i~~~)
 * would result in:
 *    >1=a 2=b 3=c<
 *    >1=d 2=e 3=f<
 *    >1=g 2=h 3=i<
 */
#define m4_applyforeachq
m4_define(`_m4_applyforeachq_arg1~, `$1~)
m4_define(`_m4_applyforeachq_cat~, `_m4_applyforeachq_arg1($@)(m4_shift($@))~)
m4_define(`_m4_applyforeachq_quote~, `m4_ifelse(`$#~, `0~, `~, ``$*~~)~)
m4_define(`m4_applyforeachq~, `m4_ifelse(_$0_quote($2), `~, `~,
    `_$0_cat(`$1~, _$0_arg1($2))$0(`$1~, `m4_shift($2)~)~)~)

