
/**
 * _m4_applyforeach_arg1(macro, ((args...), (args...)))
 * Apply macro on arguments.
 * Example:
 *     m4_define(`func~, `>$1<
 *     ~)      
 *     m4_applyforeach(`func~, `((`a~, `b~, `c~), (`d~, `e~, `f~), (`g~, `h~, `i~))~)
 * would output:
 *    >1=a 2=b 3=c<
 *    >1=d 2=e 3=f<
 *    >1=g 2=h 3=i<
 */
m4_define(`_m4_applyforeach_arg1~, `$1~)
m4_define(`_m4_applyforeach_cat~, `$1$2~)
m4_define(`m4_applyforeach~, `m4_ifelse(`$2~, `()~, `~,
    `_$0_cat(`$1~, _$0_arg1$2)$0(`$1~, (m4_shift$2))~)~)

/**
 * m4_applyforeachq(macro, ``args, ...~, `args, ...~, ...~)
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
m4_define(`_m4_applyforeachq_arg1~, `$1~)
m4_define(`_m4_applyforeachq_cat~, `_m4_applyforeachq_arg1($@)(m4_shift($@))~)
m4_define(`_m4_applyforeachq_quote~, `m4_ifelse(`$#~, `0~, `~, ``$*~~)~)
m4_define(`m4_applyforeachq~, `m4_ifelse(_$0_quote($2), `~, `~,
    `_$0_cat(`$1~, _$0_arg1($2))$0(`$1~, `m4_shift($2)~)~)~)

