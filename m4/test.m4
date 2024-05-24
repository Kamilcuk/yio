m4_divert(-1)
dnl {{{ start
dnl vim: filetype=m4 foldmethod=marker comments=s1\:/*,mb\:*,ex\:*/,\://,b\:#,\:%,\:XCOMM,n\:>,fb\:-,fb\:•,\:#,\:dnl,\:m4_dnl cindent tabstop=2 noexpandtab softtabstop=0 shiftwidth=2
/// @file

m4_changequote(,)
m4_changequote(«, »)
m4_changecom(«/*», «*/»)

dnl }}}
dnl {{{1 m4_define

/**
 * @def m4_define_function(name, «value»)
 *
 * https://www.gnu.org/software/m4/manual/m4-1.4.14/html_node/Composition.html
 * Also remove everything after ( in the name.
 */
#define j__define_function(...)
m4_define(«m4_define_function»,
	«m4_ifelse(
		«$#», «0», ««$0»»,
		«_$0(«$1», «$2», «$»«#», «$»«0»)»)»)
m4_define(«_m4_define_function»,
	«m4_define(
		m4_patsubst(«$1», «(.*»),
		«m4_ifelse(«$3», «0», ««$4»», «$2»)»)»)

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
m4_define(«m4_define_name», «m4_ifelse(«$#», «0», ««$0»»,
	«_$0(«$1», «$2», «$»«#», «$»«@»)»)»)
m4_define(«_m4_define_name», «m4_define(«$1»,
	««$2»m4_ifelse(«$3», «0», «», «($4)»)»)»)

m4_test(«m4_define_name(«nameA», «valueA»)nameA», «valueA»)
m4_test(«m4_define_name(«nameB», «valueB»)nameB(1,2,3)», «valueB(1,2,3)»)
m4_test(«m4_define_name(«nameC», «valueC»)nameC( 1 ,   2   ,  3   )», «valueC(1 ,2   ,3   )»)

dnl }}}
dnl {{{ assert error test

/**
 * @param message Message to print.
 * @param ... List of any arguments to print
 * @ingroup m4
 * @brief Just print an error and exits m4 with an error.
 */
#define m4_fatal(message, ...)
m4_define(«m4_fatal», «m4_errprint(m4___file__:m4___line__«: error: $*
»)m4_m4exit(«1»)»)

m4_define(«m4_assert_fail»,
	«m4_fatal(
«assert failed: $3$4
  "$1" != "$2"
  "»$1«" != "»$2«"
»)»)

m4_define(«m4_assert_quote», «m4_ifelse(«$#», «0», «», ««$*»»)»)

m4_define(«m4_assert»,
	«m4_ifelse(
		m4_assert_quote($1),
		m4_assert_quote($2),
		«»,
		«m4_assert_fail($@)»)»)

m4_define(«m4_assert_not»,
	«m4_ifelse(
		m4_assert_quote($1),
		m4_assert_quote($2),
		«m4_assert_fail($@)»)»)

m4_define(«m4_assert_regex»,
	«m4_assert_not(
		«m4_regexp(«$1», «$2»)»,
		«-1»,
	)»)

m4_ifdef(
	«m4_TEST»,
	«m4_define(«m4_test», «m4_assert($@)»)»,
	«m4_define(«m4_test», «»)»
)

m4_test(«a», «a»)
m4_test(«m4_patsubst(«a», «a», «\&b»)», «ab»)

dnl }}}
dnl {{{ basic utilities

m4_define(«m4_argn»,
	«m4_ifelse(
		«$1», 1, ««$2»»,
  		«m4_argn(m4_decr(«$1»), m4_shift(m4_shift($@)))»)»)

m4_define(«m4_if», «m4_ifelse(«$@»)»)

m4_define(«m4_case»,
		«m4_ifelse(
			«$#», 0, «»,
			«$#», 1, «»,
			«$#», 2, «$2»,
			«$1», «$2», «$3»,
			«$0(«$1», m4_shift(m4_shift(m4_shift($@))))»)»)

m4_define(«m4_casearg»,
		«m4_case(
			m4_shift($@),
			«m4_fatal(«$1: invalid number of arguments: $2»)»)»)

/**
 * Just ignore the arguments.
 */
#define m4_ignore(...)
m4_define(«m4_ignore», «»)

/**
 * Just ignore the arguments.
 * Used for commenting code.
 */
#define m4_ign(...)
m4_define(«m4_ign», «»)

m4_define(«m4_I», «»)

/**
 * Expand to each argument
 */
m4_define(«m4_do»,
	«m4_ifelse(
		«$#», «0», «»,
		«$#», «1», «m4_ifelse(«$1», «», «», $1)»,
		«$1«»$0(m4_shift($@))»)»)

/** concatenate all arguments */
m4_define(«m4_cat», «$1$2$3$4$5$6$7$8$9»)»)

/** ignore last argument and call first argument with the rest of arguments */
m4_define(«m4_call», «$1(m4_reverse(m4_shift(m4_reverse(m4_shift($@)))))»)

/** reverse list of arguments */
m4_define(«m4_reverse»,
		«m4_ifelse(
			«$#», «0», «»,
			«$#», «1», ««$1»»,
			«m4_reverse(m4_shift($@)),«$1»»)»)

/** a newline */
m4_define(«m4_nl», «
»)

/** like m4_elseif, but ignore last argument */
m4_define_function(«m4_callif», «m4_call(«m4_ifelse», $@)»)

dnl }}}
dnl {{{ string utilities

m4_define_function(«m4_ifmath(expr, true, false)»,
	«m4_ifelse(m4_eval(«$1»), 0, «$3», «$2»)»)

m4_define_function(«m4_ifregex(str, rgx, true, false)»,
	«m4_ifelse(m4_regexp(«$1», «$2»), -1, «$4», «$3»)»)

m4_define_function(«m4_ifsubstr(str, needle, true, false)»,
	«m4_ifelse(m4_index(«$1», «$2»), -1, «$4», «$3»)»)

m4_define(«m4_regexquote», «m4_patsubst(«$@», «[]\/$*.^[]», «\&»)»)

/** m4_quote(args) - convert args to single-quoted string *
m4_define(«m4_quote», «m4_ifelse(«$#», «0», «», ««$*»»)»)

/** m4_dquote(args) - convert args to quoted list of quoted strings */
m4_define(«m4_dquote», ««$@»»)

/** m4_dquote_elt(args) - convert args to list of double-quoted strings */
m4_define(«m4_dquote_elt», «m4_ifelse(«$#», «0», «», «$#», «1», «««$1»»»,
                             «««$1»»,$0(m4_shift($@))»)»)

/**
 * split string on spaces
 */
#define m4_split(str)
#define m4_split(str, rgx)
m4_define(«m4_split»,
	«m4_ifelse(
		«$#», «0», «»,
		«$#», «1», «$0(«$1», « »m4_nl)»,
		«$#», «2», «m4_patsubst(
				m4_patsubst(
					«$1»,
					«[$2]*\([^$2]+\)[$2]*»,
					««««\1»»,»»),
				«,$»)»,
		«m4_fatal(«invalid arguments to $0»)»)»)
m4_test(«m4_split(«a b    c»)», «««a»,«b»,«c»»»)

/**
 * join each ARG, excluding empty lines
 */
#define m4_join(sep, ...)
m4_define(«m4_join»,
	«m4_ifelse(
		«$#», «2», ««$2»»,
		«ifelse(«$2», «», «», ««$2»_»)$0(«$1», m4_shift(m4_shift($@)))»)»)
m4_define(«_m4_join»,
	«m4_ifelse(«$#$2», «2», «»,
		«m4_ifelse(«$2», «», «», ««$1$2»»)$0(«$1», m4_shift(m4_shift($@)))»)»)

/**
 * join each ARG, including empty ones,
 * into a single string, with each element separated by SEP
 */
#define m4_joinall(sep, ...)
m4_define(«m4_joinall», ««$2»_$0(«$1», m4_shift($@))»)
m4_define(«_m4_joinall», «m4_ifelse(«$#», «2», «», ««$1$3»$0(«$1», m4_shift(m4_shift($@)))»)»)

/**
 * The m4_chop macro (based on perl's chop command) returns the
 * input string minus its final character. m4_chop is useful for
 * removing "\n" from m4_esyscmd strings.
 */
#define m4_chop(string)
m4_define(«m4_chop», «m4_substr($1, 0, m4_decr(m4_len($1)))»)

/** like python rstrip */
m4_define_function(«m4_rstrip(str)», «m4_patsubst(«$1», «\s*$»)»)

/** like python lstrip */
m4_define_function(«m4_lstrip(str)», «m4_patsubst(«$1», «^\s*»)»)

/** like python strip */
m4_define_function(«m4_strip(str)», «m4_rstrip(m4_lstrip(«$1»))»)
m4_test(«m4_strip(«   a   b   »)», «a   b»)

/** like python string.count */
m4_define(«m4_count», «m4_len(m4_patsubst(«$1», «[^$2]*»))»)
m4_test(«m4_count(«abacadef», «a»)», «3»)

/** count newlines in a string */
m4_define(«m4_count_lines», «m4_count(«$@», m4_nl)»)
m4_test(«m4_count_lines(«ab
			de
			ef
			»)», «3»)

dnl }}}
dnl {{{ m4_args

m4_define_function(«m4_args_pop_back(cnt, args...)»,
	«m4_ifelse(
		«$1», 0, «m4_shift($@)»,
		«m4_reverse(
			m4_args_pop_front(
				$1,
				m4_reverse(
					m4_shift($@))))»)»)
m4_define_function(«m4_args_pop_front(cnt, args...)»,
	«m4_ifelse(
		«$1», 0, «$@»,
		«$1», 1, «m4_shift(m4_shift($@))»,
		«m4_args_pop_front(m4_eval($1 - 1), m4_shift(m4_shift($@)))»)»)
m4_define_function(«m4_args_len(args...)», «$#»)
m4_define_function(«m4_args_sub(start, len, args...)»,
	«m4_args_pop_back(
		m4_max(m4_eval($# - 2 - $1 - $2), 0),
		m4_args_pop_front($1, m4_shift(m4_shift($@))))»)
m4_define(«m4_args_first», «$1»)
m4_define(«m4_args_esc», «$@»)

dnl }}}
dnl {{{ m4_tuple

m4_define_function(«m4_tuple_join(tuple1, tuple2)»,
		«(m4_do(
				«m4_cat(«m4_args_esc», m4_strip(«$1»))»,
				«m4_ifelse(
					m4_strip(«$1»), «()», «»,
					m4_strip(«$2»), «()», «»,
					«,»)»,
				«m4_cat(«m4_args_esc», m4_strip(«$2»))»,
				))»)
m4_test(«m4_tuple_join( (a,b,c), (1,2,3) )», «(a,b,c,1,2,3)»)

m4_define_function(«m4_tuple_first(tuple)», «m4_args_first$1»)
m4_define_function(«m4_tuple_shift(tuple)», «(m4_cat(«m4_shift», m4_rstrip(«$1»)))»)
m4_define_function(«m4_tuple_len(tuple)», «m4_cat(«m4_args_len», m4_rstrip(«$1»))»)
m4_define_function(«m4_tuple_isempty(tuple)», «m4_ifelse(m4_rstrip(«$2»), «()», «1», «0»)»)
m4_define(«m4_tuples_merge», «_m4_tuples_merge((), $@, «»)»)
m4_define(«_m4_tuples_merge»,
	«m4_callif(
		«$2», «», «$1»,
		m4_tuple_isempty(«$2»), «1», «»,
		«m4_do(
			«$0(m4_tuple_join($1, m4_tuple_first($2)), m4_shift(m4_shift($@)))»,
			«m4_callif(
				m4_tuple_len(«$2»), 0, «»,
				m4_tuple_len(«$2»), 1, «»,
				«$0($1, m4_tuple_shift($2), m4_shift(m4_shift($@)))»,
			)»,
		)»,
	)»)


dnl }}}
dnl {{{ m4 foreach

/**
 * m4_foreach(x, (item_1, item_2, ..., item_n), stmt)
 *
 * https://www.gnu.org/software/m4/manual/m4-1.4.14/html_node/Foreach.html
 */
#define m4_foreach(iterator, braces_item_list, statement)
m4_define(«m4_foreach», «m4_pushdef(«$1»)_m4_foreach($@)m4_popdef(«$1»)»)
m4_define(«_m4_foreach_arg1», «$1»)
m4_define(«_m4_foreach», «m4_ifelse(«$2», «()», «»,
  «m4_define_name(«$1», _m4_foreach_arg1$2)$3«»$0(«$1», (m4_shift$2), «$3»)»)»)

«/**
 * @ingroup m4
 * @param macro A macro name to apply arguments to
 * @param argslist A list og macro arguments in the form of
 * «((arg1, arg2, ...), (args1, arg2, ...), ...)».
 * Remember to qoute it!!
 * @param separator An optional separator to separate elements.
 *
 * Apply macro on arguments in brackets.
 * The list of arguments is shifted and is applied on each argument.
 * Example:
 *     m4_define(«m4_func», «>1=$1 2=$2 3=$3<») m4_applyforeach(«m4_func», «((«a», «b», «c»), («d», «e», «f»), («g», «h», «i»))», « % »)
 * would output:
 *    >1=a 2=b 3=c< % >1=d 2=e 3=f< % >1=g 2=h 3=i<
 */
#define m4_applyforeach(macro, args, separator) »
m4_define(«_m4_applyforeach_arg1», «$1»)
m4_define(«_m4_applyforeach_cat», «$1$2»)
m4_define(«_m4_applyforeach_separator», «m4_ifelse(«$2», «()», «», «$1»)»)
m4_define(«m4_applyforeach», «m4_ifelse(«$2», «()», «»,
    «_m4_applyforeach_cat(
    	«$1»,
    	_m4_applyforeach_arg1$2)_m4_applyforeach_separator(
    		«$3»,
    		(m4_shift$2))m4_applyforeach(
    			«$1»,
    			(m4_shift$2),
    			«$3»)»)»)
m4_test(
		«m4_define(«m4_func», «>1=$1 2=$2 3=$3<»)m4_applyforeach(«m4_func», «((«a», «b», «c»), («d», «e», «f»), («g», «h», «i»))», « % »)»,
		«>1=a 2=b 3=c< % >1=d 2=e 3=f< % >1=g 2=h 3=i<»)
m4_test(
		«m4_define(«m4_func», «@$1<»)m4_applyforeach(«m4_func», «((bar), (foo))», «:»)»,
		«@bar<:@foo<»)
m4_test(
		«m4_define(«m4_func», «@$1<»)m4_applyforeach(«m4_func», «((bar), (foo),)», «:»)»,
		«@bar<:@foo<»)

/**
 * @ingroup m4
 *
 * Example:
 *
 *     m4_applyforeachdefine(«((1, 2), (3, 4))», «one=$1 two=$2 »)
 */
#define m4_applyforeachdefine(bracket_list, function_body, separator)

m4_define(«m4_applyforeachdefine»,
		«m4_pushdef(
			«_$0_function»,
			«$2»)m4_applyforeach(
				«_$0_function»,
				«$1»,
				«$3»)m4_popdef(
					«_$0_function»)»)

m4_test(
		«m4_applyforeachdefine(«((1, 2), (3, 4))», «one=$1 two=$2», «,»)»,
		«one=1 two=2,one=3 two=4»)
m4_test(
		«m4_applyforeachdefine(«((1, 2), (3, 4),)», «one=$1 two=$2», «,»)»,
		«one=1 two=2,one=3 two=4»)

/**
 * @ingroup m4
 *
 * Example:
 *
 *     m4_applyforeachqdefine(«««1», «2»», ««3», «4»»», «one=$1 two=$2 »)
 */
#define m4_applyforeachqdefine(qouted_list, function_body, separator)

m4_define(«m4_applyforeachqdefine», «m4_pushdef(
	«_$0_function»,
	«$2»)m4_applyforeachq(
		«_$0_function»,
		«$1»,
		«$3»)m4_popdef(
			«_$0_function»)»)

m4_test(«m4_applyforeachqdefine(«««1», «2»», ««3», «4»»», «one=$1 two=$2», «,»)»)
// PASS_REGULAR_EXPRESSION one=1 two=2,one=3 two=4
m4_test(«m4_applyforeachqdefine(«a, b», «>>$1<<»)»)
// PASS_REGULAR_EXPRESSION >>a<<>>b<<

«/**
 * @ingroup m4
 *
 * Apply macro on quoted list
 * Example:
 *    m4_define(«m4_func», «>1=$1 2=$2 3=$3<
 *    »)
 *    m4_applyforeachq(«m4_func», «««a», «b», «c»», ««d», «e», «f»», ««g», «h», «i»»»)
 * would result in:
 *    >1=a 2=b 3=c<
 *    >1=d 2=e 3=f<
 *    >1=g 2=h 3=i<
 */
#define m4_applyforeachq(function, qouted_list, separator) »

m4_define(«_m4_applyforeachq_arg1», «$1»)
m4_define(«_m4_applyforeachq_cat», «_m4_applyforeachq_arg1($@)(m4_shift($@))»)
m4_define(«_m4_applyforeachq_quote», «m4_ifelse(«$#», «0», «», ««$*»»)»)
m4_define(«_m4_applyforeachq_separator», «m4_ifelse(m4_eval(«$# > 2»), «1», «$1», «»)»)
m4_define(«m4_applyforeachq», «m4_ifelse(
		_$0_quote($2),
		«»,
		«»,
		«_$0_cat(
				«$1»,
				_$0_arg1($2))_$0_separator(
						«$3»,
						$2)«»$0(
								«$1»,
								«m4_shift($2)»,
								«$3»)»)»)

m4_test(«m4_define(«m4_func», «>1=$1 2=$2 3=$3<»)m4_applyforeachq(«m4_func», «
		a,
		d,
		g»)»)
// PASS_REGULAR_EXPRESSION >1=a 2= 3=<>1=d 2= 3=<>1=g 2= 3=<
m4_test(«m4_define(«m4_func», «>1=$1 2=$2 3=$3<»)m4_applyforeachq(«m4_func», «
		z,
		x,
		y», « % »)»)
// PASS_REGULAR_EXPRESSION >1=z 2= 3=< % >1=x 2= 3=< % >1=y 2= 3=<
m4_test(«m4_define(«m4_func», «>1=$1 2=$2 3=$3<»)m4_applyforeachq(«m4_func», ««1», «2», «3»», « % »)»)
// PASS_REGULAR_EXPRESSION >1=1 2= 3=< % >1=2 2= 3=< % >1=3 2= 3=<
m4_test(«m4_define(«m4_func», «>1=$1 2=$2 3=$3<»)m4_dnl
m4_applyforeachq(«m4_func», «««a», «b», «c»», ««d», «e», «f»», ««g», «h», «i»»», « % »)»)
// PASS_REGULAR_EXPRESSION >1=a 2=b 3=c< % >1=d 2=e 3=f< % >1=g 2=h 3=i<

«/**
 * @def m4_applyforloopdefine(start_range, end_range, function_body, separator)
 * @ingroup m4
 * @param start_range A number to start counting from, inclusive.
 * @param end_range A number to end counting on, inclusive.
 * @param function_body The body of a function to call. $1 will be substituted for the number
 * @param separator An optional separator to call.
 *
 * Defines a temporary function with the function body passed
 * as a paremeter. Generates numbers from start_range to end_range
 * and passes the numbe as the first (and only) argument of the
 * defined function. Optionally non-empty separator can be used
 * to separate elements.
 */
#define m4_applyforloopdefine(start_range, end_range, function_body, separator) »

m4_define(«m4_applyforloopdefine», «m4_pushdef(
	«_$0_function»,
	«$3»)m4_forloop(
		«_$0_iterator»,
		«$1»,
		«$2»,
		«_$0_function(_$0_iterator)»,
		«$4»)m4_popdef(
			«_$0_function»)»)

m4_test(«m4_applyforloopdefine(1, 3, «arg=$1», « S »)»)
// PASS_REGULAR_EXPRESSION arg=1 S arg=2 S arg=3
m4_test(«m4_applyforloopdefine(1, 3, «ARG=$1 »)»)
// PASS_REGULAR_EXPRESSION ARG=1 ARG=2 ARG=3« »*
// Recursive calls have to be defined beforehand
m4_test(«m4_define(«m4_func», «cnt=$1»)m4_applyforloopdefine(1, 3, «m4_applyforloop(1, $1, «m4_func», «,»)», « S »)»)
// PASS_REGULAR_EXPRESSION cnt=1 S cnt=1,cnt=2 S cnt=1,cnt=2,cnt=3



«/**
 * @ingroup m4
 */
#define m4_applyforloop(...) »
m4_define(«m4_applyforloop», «m4_forloop(
	«_m4_applyforloop_iterator»,
	«$1»,
	«$2»,
	«$3(_m4_applyforloop_iterator)»,
	«$4»)»)

m4_test(«m4_define(«m4_func», «m4_forloop(«J», 1, $1, «@$1, J%»)»)m4_applyforloop(1, 4, «m4_func»)»)
// PASS_REGULAR_EXPRESSION @1, 1%@2, 1%@2, 2%@3, 1%@3, 2%@3, 3%@4, 1%@4, 2%@4, 3%@4, 4%

dnl }}}
dnl {{{ subprocess

m4_define(«m4_shsplit_sh»,
	«m4_patsubst(
		m4_esyscmd(«printf "%s" »m4_shquote($1)« | xargs printf "«««%s»»,»"»),
		«,$»)»)

m4_define_function(«m4_erun», «m4_esyscmd(m4_shquote($@))»)

m4_define_function(«m4_run», «m4_syscmd(m4_shquote($@))»)

m4_define_function(«m4_redir», «m4_syscmd(m4_shquote(m4_shift($@)) « >> » m4_shquote(«$1»))»)

/**
 * @brief Quote the argument according to the shell.
 */
#define m4_shqoute(...)
m4_define(«m4_shquote», «m4_applyforeachq(«_m4_shquote», «$@», « »)»)
m4_define(«_m4_shquote», «'m4_patsubst(«$1», «'», «'\\''»)'»)

dnl }}}
dnl {{{ path

/**
 * @ingroup m4
 * @return The filename from __file__
 */
#define m4_filename(...)
m4_define(«m4_filename», «m4_patsubst(m4___file__, «^.*\(/\|\\\)»)»)

m4_define_function(«m4_write», «m4_redir(«$1», «printf», «%s», «$2»)»)

m4_define_function(«m4_read», «m4_erun(«cat», «$1»)»)

dnl }}}
dnl {{{ forloop

/**
 * @ingroup m4
 * For each number between @b start_range to @b end_range
 * define @b identifier to dash joined with the number
 * and place @b replacement string joined with the separator.
 */
#define m4_forloopdash
m4_define_function(«m4_forloopdash»,
	«m4_ifelse(
		m4_eval(«($2) <= ($3)»),
		«1»,
		«m4_pushdef(«$1»)_m4_forloopdash(
			m4_eval(«$2»),
			m4_eval(«$3»),
			«m4_define(«$1»,»,
			«)$4»,
			«$5»,
			«m4_incr»)m4_popdef(«$1»)»,
		«m4_ifelse(
			m4_eval(«($2) >= ($3)»),
			«1»,
			«m4_pushdef(«$1»)_m4_forloopdash(
				m4_eval(«$2»),
				m4_eval(«$3»),
				«m4_define(«$1»,»,
				«)$4»,
				«$5»,
				«m4_decr»)m4_popdef(«$1»)»,
			«»m4_dnl
)»m4_dnl
)»m4_dnl
)m4_dnl;

/**
 * @ingroup m4
 * Internal function for m4_forloopdash
 */
#define _m4_forloopdash
m4_define(«_m4_forloopdash»,
	«$3«_$1»$4«»m4_ifelse(
		«$1»,
		«$2»,
		«»,
		«$5$0($6(«$1»),«$2»,«$3»,«$4»,«$5»,«$6»)»m4_dnl
)»m4_dnl
)m4_dnl;


«/**
 * @brief Substitute for count times the replacement with leading dash.
 * @ingroup m4
 *
 * Replace X in substitution_with_X for the number with added leading dash
 * and join the replacement with the separator
 * for each number within (start_range, end_range).
 */
#define m4_forloopdashX(begin_range, end_range, template_with_X, separator) »
m4_define_function(«m4_forloopdashX», «m4_forloopdash(«X», «$1», «$2», «$3», «$4»)»)m4_dnl;

m4_test(«m4_forloopdashX(1, 5, ««hello»X», «, »)»)
// PASS_REGULAR_EXPRESSION hello_1, hello_2, hello_3, hello_4, hello_5
m4_test(«m4_forloopdashX(1, 5, ««hello»X»)»)
// PASS_REGULAR_EXPRESSION hello_1hello_2hello_3hello_4hello_5

«/**
 * @ingroup m4
 * @brief @see m4_forloopdashX
 */
#define m4_forloopdashI(begin_range, end_range, template_with_X, separator) »
m4_define_function(«m4_forloopdashI», «m4_forloopdash(«I», «$1», «$2», «$3», «$4»)»)m4_dnl;

«/**
 * @ingroup m4
 * @brief @see m4_forloopdashX
 */
#define m4_forloopdashY(begin_range, end_range, template_with_X, separator) »
m4_define_function(«m4_forloopdashY», «m4_forloopdash(«Y», «$1», «$2», «$3», «$4»)»)m4_dnl;


/**
 * @ingroup m4
 * Generates iterators from start_range to end_range substitution
 * the iterator in substitution string separating substitutiong
 * by the separator.
 */
#define m4_forloop(i, 1, 1)
m4_define_function(«m4_forloop»,
	«m4_ifelse(
		m4_eval(«($2) <= ($3)»),
		«1»,
		«m4_pushdef(«$1»)_m4_forloop(
			m4_eval(«$2»),
			m4_eval(«$3»),
			«m4_define(«$1»,»,
			«)$4»,
			«$5»,
			«m4_incr»)m4_popdef(«$1»)»,
		«m4_ifelse(
			m4_eval(«($2) >= ($3)»),
			«1»,
			«m4_pushdef(«$1»)_m4_forloop(
				m4_eval(«$2»),
				m4_eval(«$3»),
				«m4_define(«$1»,»,
				«)$4»,
				«$5»,
				«m4_decr»)m4_popdef(«$1»)»,
			«»)»)»)

/**
 * @ingroup m4
 * Internal function
 */
m4_define_function(«_m4_forloop»,
	«$3«$1»$4«»m4_ifelse(
		«$1»,
		«$2»,
		«»,
		«$5$0($6(«$1»),«$2»,«$3»,«$4»,«$5»,«$6»)»)»)

/**
 * @ingroup m4
 * @brief substitute for count times the replacement.
 *
 * Replace X in substitution_with_X and join with separator
 * for each number within (start_range, end_range)
 * Example:
 *     m4_forloopX(1, 5, ««hello_»X», «, »)
 * results in:
 *     hello_1, hello_2, hello_3, hello_4, hello_5
 * Example:
 *     m4_forloopX(1, 5, ««hello_»X»)
 * results in:
 *     hello_1hello_2hello_3hello_4hello_5
 */
#define m4_forloopX(1, 1, ...)
m4_define_function(«m4_forloopX»,«m4_forloop(«X»,«$1»,«$2»,«$3»,«$4»)»)

/**
 * @ingroup m4
 * @brief @see m4_forloopX
 */
#define m4_forloopI(1, 1, ...)
m4_define_function(«m4_forloopI»,«m4_forloop(«I»,«$1»,«$2»,«$3»,«$4»)»)

/**
 * @ingroup m4
 * @brief @see m4_forloopX
 */
#define m4_forloopY(1, 1, ...)
m4_define_function(«m4_forloopY»,«m4_forloop(«Y»,«$1»,«$2»,«$3»,«$4»)»)

dnl }}}
dnl {{{ math

m4_define_function(«m4_max(numbers...)»,
	«m4_case(«$#»,
		«0», «»,
		«1», «$1»,
		«2», «m4_ifmath(«$1 > $2», «$1», «$2»)»,
		«$0($0($1, $2), m4_shift(m4_shift($@)))»)»)

dnl }}}
dnl {{{ include

#define m4_include_relative »
m4_define_function(«m4_include_relative»,
«m4_include(m4_patsubst(m4___file__, «/«^/»*$»)/$1)»)

#define m4_include_relative_quiet »
m4_define_function(«m4_include_relative_quiet»,
«m4_divert(-1)m4_include_relative(«$@»)m4_divert(0)»)

dnl }}}
dnl {{{ seq

«/**
 * @brief Generates numbers with separated by a comma.
 * @param start_number Starting number of range, inclusive
 * @param end_number Ending number of range, inclusive
 * @param variable Variable to substitute in replacement string
 * @param replacement The replacement string
 * @param separator The separator.
 * @ingroup m4
 */
#define m4_seqcomma(start_number, end_number, variable, replacement, separator) »
m4_define_function(«m4_seqcomma»,
	«m4_forloop(
		m4_ifelse(«$3», «», «_m4_sEqCoMmA_vArIaBlE», «$3»),
		«$1»,
		«$2»,
		«m4_ifelse(«$4», «», «m4_ifelse(«$3», «», «_m4_sEqCoMmA_vArIaBlE», «$3»)», «$4»)»,
		«m4_ifelse(«$5», «», «,», «$5»)»)»)m4_dnl;
m4_test(«m4_seqcomma(1, 5)»)m4_dnl;
// PASS_REGULAR_EXPRESSION 1,2,3,4,5

«/**
 * @ingroup m4
 * generates numbers with a leading dash separated by a comm
 */
#define m4_seqdashcomma »
m4_define_function(«m4_seqdashcomma»,
	«m4_forloopdash(
		m4_ifelse(«$3», «», «_m4_sEqCoMmA_vArIaBlE», «$3»),
		«$1»,
		«$2»,
		«m4_ifelse(«$4», «», «m4_ifelse(«$3», «», «_m4_sEqCoMmA_vArIaBlE», «$3»)», «$4»)»,
		«m4_ifelse(«$5», «», «,», «$5»)»)»)
m4_test(«m4_seqdashcomma(1, 5)»)m4_dnl;
// PASS_REGULAR_EXPRESSION _1,_2,_3,_4,_5

«/**
 * @ingroup m4
 * @see m4_seqcomma
 */
#define m4_seqcommaX(start_range, stop_range, replacement_with_X, separator) »
m4_define_function(«m4_seqcommaX», «m4_seqcomma(«$1», «$2», «X», «$3», «$4»)»)m4_dnl;

«/**
 * @def m4_seqdashcommaX(start_number, end_number, replacement_with_X, separator)
 * @ingroup m4
 * @see m4_seqdashcomma
 */
#define m4_seqdashcommaX »
m4_define(«m4_seqdashcommaX», «m4_seqdashcomma(«$1», «$2», «X», «$3», «$4»)»)m4_dnl;
m4_test(«m4_seqdashcommaX(1, 5, «hello«»X»)»)m4_dnl;
// PASS_REGULAR_EXPRESSION hello_1,hello_2,hello_3,hello_4,hello_5

«/**
 * @see m4_seqcomma
 */
#define m4_seqcommaY »
m4_define(«m4_seqcommaY», «m4_seqcomma(«$1», «$2», «Y», «$3», «$4»)»)m4_dnl;

«/**
 * @ingroup m4
 * @see m4_seqdashcomma
 */
#define m4_seqdashcommaY »
m4_define_function(«m4_seqdashcommaY», «m4_seqdashcomma(«$1», «$2», «Y», «$3», «$4»)»)m4_dnl;

«/**
 * @ingroup m4
 * @see m4_seqcomma
 */
#define m4_seqcommaI »
m4_define_function(«m4_seqcommaI», «m4_seqcomma(«$1», «$2», «I», «$3», «$4»)»)m4_dnl;

«/**
 * @ingroup m4
 * @see m4_seqdashcomma
 */
#define m4_seqdashcommaI »
m4_define_function(«m4_seqdashcommaI», «m4_seqdashcomma(«$1», «$2», «I», «$3», «$4»)»)m4_dnl;

dnl }}}
dnl {{{ misc

/**
 * Synchronize the lines.
 * @param inc Increment the current line number by this value.
 */
#define m4_syncline_in(inc)
m4_define_function(«m4_syncline_in»,
«#line AA m4_eval(m4___line__ m4_ifelse(«$1», «», «», « + $1»)) "m4___file__"
»)

/**
 * Same as m4_syncline_in, but expands to nothing if not debug.
 * @see m4_syncline_in
 * @param inc
 */
#define m4_syncline(inc)
m4_define_function(«m4_syncline», «m4_ifdef(«m4_DEBUG», «m4_syncline_in($@)»)»)

#define m4_syncline_dnl(inc)
m4_define_function(«m4_syncline_dnl», «m4_syncline($@)m4_dnl »)

m4_define(«m4_S»,
«#line m4_eval($2 + m4___line__) "m4___file__"m4_ifelse(«$1», «», «», «$1
#line m4_eval($2 + m4___line__ + m4_count_lines(«$1»)) "m4___file__"
»)»)

m4_define(«m4_sdivert», «m4_divert(m4_ifelse($#, 0, 0, $@))m4_S(,1)m4_dnl»)

dnl }}}
m4_divert(0)m4_dnl
m4_traceon(«_m4_tuples_merage»)
m4_tuple_len((a,b,c))
m4_tuples_merge( ((a,b,c)), ((d,e,f),(4,5,6),()) )
