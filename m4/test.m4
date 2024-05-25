m4_divert(-1)
dnl {{{ start
dnl vim: filetype=m4 foldmethod=marker comments=s1\:/*,mb\:*,ex\:*/,\://,b\:#,\:%,\:XCOMM,n\:>,fb\:-,fb\:•,\:#,\:dnl,\:m4_dnl cindent tabstop=2 noexpandtab softtabstop=0 shiftwidth=2 list
/// @file

m4_include(init.m4)

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

m4_define_function(«m4_assert_fail(expr1, expr2, str1, str2, ...)»,
	«m4_fatal(
«assert failed: $5$6$7$8$9
  "$3" != "$4"
  "$1" != "$2"
»)»)

m4_define(«m4_assert_quote», «m4_ifelse(«$#», «0», «», ««$*»»)»)

m4_define_function(«_m4_assert(expr1, expr2, str1, str2, ...)»,
	«m4_ifelse(«$1», «$2», «», «m4_assert_fail($@)»)»)
m4_define(«m4_assert», «_m4_assert($1, $2, $@)»)

m4_define_function(«_m4_assert_not(expr1, expr2, str1, str2, ...)»,
	«m4_ifelse(«$1», «$2», «m4_assert_fail($@)»)»)
m4_define_function(«m4_assert_not», «_$0($1, $2, $@)»)

#define m4_test(expr, rgx)
m4_define(«_m4_test_in», «m4_errprint(m4___file__:m4___line__«: "$3" -> "$1" == "$2"
»)_m4_assert($@)»)
m4_ifdef(
	«m4_TEST»,
	«m4_define(«m4_test», «_m4_test_in(($1), ($2), $@)»)»,
	«m4_define(«m4_test», «»)»
)
m4_test(«a», «a»)
m4_test(«m4_patsubst(«a», «a», «\&b»)», «ab»)

dnl }}}
dnl {{{ basic utilities

m4_define(«m4_shift2», «m4_shift(m4_shift($@))»)

m4_define(«m4_shift3», «m4_shift(m4_shift(m4_shift($@)))»)

m4_define(«m4_shift4», «m4_shift(m4_shift(m4_shift(m4_shift($@))))»)

m4_define(«m4_shift5», «m4_shift(m4_shift(m4_shift(m4_shift(m4_shift($@)))))»)

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
		«$#», «1», «m4_ifelse(«$1», «», «», «$1»)»,
		«$1«»$0(m4_shift($@))»)»)

/** concatenate all arguments */
m4_define(«m4_cat», «$1$2$3$4$5$6$7$8$9»)»)

m4_define(«m4_rshift»,
		«m4_ifelse(
			«$#», 0, «»,
			«$#», 1, «»,
			«$#», 2, «$1»,
			«$1,$0(m4_shift($@))»)»)
m4_test(«m4_rshift(1,2,3)», «1,2»)

/** ignore last argument and call first argument with the rest of arguments */
m4_define(«m4_call», «$1(m4_rshift(m4_shift($@)))»)

/** like m4_elseif, but ignore last argument */
m4_define_function(«m4_callif», «m4_call(«m4_ifelse», $@)»)
m4_test(«m4_callif(1, 1, 1, 2, 2, 2, 3, )», «1»)

/** reverse list of arguments */
m4_define(«m4_reverse»,
		«m4_ifelse(
			«$#», «0», «»,
			«$#», «1», ««$1»»,
			«m4_reverse(m4_shift($@)),«$1»»)»)

/** a newline */
m4_define(«m4_nl», «
»)

dnl }}}
dnl {{{ string utilities

m4_define_function(«m4_ifmath(expr, true, false)»,
	«m4_ifelse(m4_eval(«$1»), 0, «$3», «$2»)»)

m4_define_function(«m4_ifregex(str, rgx, true, false)»,
	«m4_ifelse(m4_regexp(«$1», «$2»), -1, «$4», «$3»)»)

m4_define_function(«m4_ifsubstr(str, needle, true, false)»,
	«m4_ifelse(m4_index(«$1», «$2»), -1, «$4», «$3»)»)

m4_define(«m4_regexquote», «m4_patsubst(«$@», «[]\/$*.^[]», «\&»)»)

/** m4_quote(args) - convert args to single-quoted string */
m4_define(«m4_quote», «m4_ifelse(«$#», «0», «», ««$*»»)»)
m4_test(«m4_quote(a)», ««a»»)

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

m4_define_function(«m4_rindex(str, needle)»,
		«m4_ifelse(
			m4_index(«$1», «$2»), -1, -1,
			«m4_decr(m4_len(m4_patsubst(«$1», «^\(\(.*$2\)+\).*», ««\1»»)))»)»)
m4_define_function(«_m4_rindex(pos, str, needle)»,
		«m4_ifelse(
			m4_eval(«$1 < 0»), «1», «-1»,
			m4_substr(«$2», «$1», m4_len(«$3»)), «$3», «$1»,
			«$a0(m4_decr(«$1»), «$2», «$3»)»)»)
m4_test(«m4_rindex(«abc», «b»)», «1»)
m4_test(«m4_rindex(«)», «)»)», «0»)
m4_test(«m4_rindex(«,,,,», «,»)», «3»)
m4_test(«m4_rindex(«abc», «d»)», «-1»)
m4_test(«m4_rindex(«abc», «c»)», «2»)
m4_test(«m4_rindex(«abc», «a»)», «0»)
m4_test(«m4_rindex(«a,b,c», «b»)», «2»)
m4_test(«m4_rindex(«a,b,c», «d»)», «-1»)
m4_test(«m4_rindex(««a(a»», ««»»)», «-1»)

dnl }}}
dnl {{{ m4_args

m4_define_function(«m4_args_pop_back_n(cnt, args...)»,
	«m4_ifelse(
		m4_eval(«$1 < 0»), «1», «m4_fatal(«$0 cnt=$1 < 0»)»,
		m4_eval(«$1 == 0»), «1», «m4_shift($@)»,
		m4_eval(«$# <= $1 + 1»), «1», «»,
		m4_eval(«$# == $1 + 2»), «1», «$2»,
		«$2,$0($1, m4_shift2($@))»)»)
m4_test(«m4_args_pop_back_n(0,1,2,3,4)», «1,2,3,4»)
m4_test(«m4_args_pop_back_n(1,1,2,3,4)», «1,2,3»)
m4_test(«m4_args_pop_back_n(2,1,2,3,4)», «1,2»)
m4_test(«m4_args_pop_back_n(4,1,2,3,4)», «»)

m4_define_function(«m4_args_pop_back(args...)», «m4_args_pop_back(1, $@)»)

m4_define_function(«m4_args_pop_front_n(cnt, args...)»,
	«m4_ifelse(
		«$1», 0, «$@»,
		«$1», 1, «m4_shift(m4_shift($@))»,
		«m4_args_pop_front_n(m4_eval($1 - 1), m4_shift(m4_shift($@)))»)»)

m4_deifne_function(«m4_args_pop_fron(args...)», «m4_shift($@)»)

m4_define_function(«m4_args_len(args...)», «$#»)

m4_define_function(«m4_args_sub(start, len, args...)»,
	«m4_args_pop_back(
		m4_max(m4_eval($# - 2 - $1 - $2), 0),
		m4_args_pop_front_n($1, m4_shift(m4_shift($@))))»)

m4_define(«m4_args_first», «$1»)

m4_define(«m4_args_esc», «$@»)

dnl }}}
dnl {{{ m4_tuple

m4_define(«m4_tuple_L», «(»)
m4_define(«m4_tuple_R», «)»)

m4_define_function(«m4_tuple_sane(tuple)», «_m4_tuple_sane(m4_strip(«$1»))»)
m4_define_function(«m4_tuple_sane(tuple)», «m4_ifelse(«$1», «», «()», «m4_strip(«$1»)»)»)
m4_define_function(«_m4_tuple_sane(tuple)»,
		«m4_ifelse(
			m4_eval(«$# != 1»), «1», «m4_fatal(«m4_tuple_sane: wrong number of arguments: $#»)»,
			«$1», «», «()»,
			m4_regexp(«$1», «^(.*)$»), «-1», «m4_fatal(«not a valid tuple: "$1"»)»,
			«$1»)»)

m4_define_function(«m4_tuple_call(func, tuple)», «m4_cat(«$1», m4_tuple_sane(«$2»))»)

m4_define_function(«m4_tuple_first(tuple)», «m4_tuple_call(«m4_args_first», «$1»)»)

m4_define_function(«m4_tuple_shift(tuple)», «(m4_tuple_call(«m4_shift», «$1»))»)

m4_define_function(«m4_tuple_len(tuple)», «m4_tuple_call(«m4_args_len», «$1»)»)
m4_test(«m4_tuple_len((1,2))», «2»)
m4_test(«m4_tuple_len((1,2,3,4,5))», «5»)

m4_define_function(«m4_tuple_isempty(tuple)», «m4_ifelse(m4_tuple_sane(«$1»), «()», «1», «0»)»)
m4_test(«m4_tuple_isempty(())», 1)
m4_test(«m4_tuple_isempty((1,2))», 0)

m4_define_function(«m4_tuple_push_back(tuple, elem, ...)»,
		«(m4_ifelse(
				m4_tuple_sane(«$1»), «()», «$2»,
				«m4_tuple_call(«m4_args_esc», «$1»),m4_shift($@)»))»)
m4_test(«m4_tuple_push_back(,1)», «(1)»)
m4_test(«m4_tuple_push_back((),1)», «(1)»)
m4_test(«m4_tuple_push_back((1),2)», «(1,2)»)
m4_test(«m4_tuple_push_back((1,2,3),4)», «(1,2,3,4)»)

m4_define_function(«m4_tuple_pop_back(tuple)», «m4_args_pop_back$1»)

m4_define_function(«m4_tuple_join(tuple1, tuple2)»,
		«(m4_do(
				«m4_tuple_call(«m4_args_esc», «$1»)»,
				«m4_ifelse(
					m4_tuple_sane(«$1»), «()», «»,
					m4_tuple_sane(«$2»), «()», «»,
					«,»)»,
				«m4_tuple_call(«m4_args_esc», «$2»)»,
				))»)
m4_test(«m4_tuple_join( (a,b,c), (1,2,3) )», «(a,b,c,1,2,3)»)
m4_test(«m4_tuple_join( (a,b,c), () )», «(a,b,c)»)
m4_test(«m4_tuple_join( (), (1,2,3) )», «(1,2,3)»)

#define m4_tuples_merge(tuple1, tuple2)
m4_define(«m4_tuples_merge», «(_m4_tuples_merge((), $@, «»))»)
m4_define_function(«_m4_tuples_merge(result, tuple1, tuple2, empty)»,
	«m4_ifelse(
		«$2», «», «$1»,
		m4_strip(«$2»), «()», «$1»,
		«m4_do(
			«$0(m4_tuple_join($1, m4_tuple_first($2)), m4_shift2($@))»,
			«m4_ifelse(
				m4_tuple_len(«$2»), 0, «»,
				m4_tuple_len(«$2»), 1, «»,
				«,$0($1, m4_tuple_shift($2), m4_shift2($@))»)»)»)»)
m4_test(
		«m4_tuples_merge( ((1,2),(a,b)), ((3,4),(c,d)))»,
		«((1,2,3,4),(1,2,c,d),(a,b,3,4),(a,b,c,d))»)
m4_test(
		«m4_tuples_merge( ((1,2),(a,b)), ((3,4),(d)))»,
		«((1,2,3,4),(1,2,d),(a,b,3,4),(a,b,d))»)
m4_test(
		«m4_tuples_merge( ((1,2),(a,b)), ((3,4),(c,d)), ((5,6),(e,f)))»,
		«((1,2,3,4,5,6),(1,2,3,4,e,f),(1,2,c,d,5,6),(1,2,c,d,e,f),(a,b,3,4,5,6),(a,b,3,4,e,f),(a,b,c,d,5,6),(a,b,c,d,e,f))»)

#define m4_tuple_has(tuple, elem)
m4_define_function(«m4_tuple_has(tuple, elem)»,
	«m4_ifelse(
		m4_tuple_sane(«$1»), «()», «0»,
		m4_tuple_first(«$1»), «$2», «1»,
		«$0(m4_tuple_shift(«$1»), $2)»)»)
m4_test(«m4_tuple_has((1,2,3,4), 5)», «0»)
m4_test(«m4_tuple_has((1,2,3,4), 2)», «1»)

m4_define_function(«m4_tuples_next(tuple, func)»,
	«m4_ifelse(
		m4_tuple_sane(«$1»), «()», «»,
		m4_cat(«$2», m4_tuple_first(«$1»)), «1», «m4_tuple_first(«$1»)»,
		«$0(m4_tuple_shift(«$1»), «$2»)»)»)

m4_define_function(«m4_tuples_next_define(tuple, expr)»,
	«m4_do(
		m4_pushdef(«_$0», «$2»),
		m4_tuples_next(«$1», «_$0»),
		m4_popdef(«_$0»),
	)»)
m4_test(
		«m4_tuples_next_define(
			«((1,2),(3,4),(5,6))»,
			«m4_eval($1 == 5 && $2 == 6)»)»,
		«(5,6)»)

#define m4_tuple_map(«tuple», «func»)
m4_define(«m4_tuple_map», «(_$0(m4_tuple_len(«$1»), «$1», «$2»))»)
m4_define(«_m4_tuple_map»,
		«m4_ifelse(
			«$1», «0», «»,
			«$3(m4_tuple_first(«$2»))m4_ifelse(
					«$1», «1», «»,
					«,$0(m4_decr(«$1»), m4_tuple_shift(«$2»), «$3»)»)»)»)»)
m4_traceon(«m4_tuple_map», «_m4_tuple_map», «func»)
m4_test(
		«m4_do(
			«m4_define(«func»,
				«m4_ifelse(
					«m4_eval($# != 1)», «1», «m4_fatal(«not enough args: $@»)»,
					«m4_eval($1 * 2)»)»)»,
			«m4_tuple_map(«(1, 2, 3)», «func»)»,
		)»,
		«(2,4,6)»)

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

m4_test(«m4_forloopdashX(1, 5, ««hello»X», «, »)», «hello_1, hello_2, hello_3, hello_4, hello_5»)
m4_test(«m4_forloopdashX(1, 5, ««hello»X»)», «hello_1hello_2hello_3hello_4hello_5»)

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
#define m4_forloop(name, start, stop, subst, separator)
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
m4_define_function(«_m4_forloop»,
	«$3«$1»$4«»m4_ifelse(
		«$1»,
		«$2»,
		«»,
		«$5$0($6(«$1»),«$2»,«$3»,«$4»,«$5»,«$6»)»)»)
m4_test(«m4_forloop(«i», 1, 3, «i», «,»)», «1,2,3»)
m4_test(«m4_forloop(«i», 1, 3, «a i»)», «a 1a 2a 3»)
m4_test(«m4_forloop(«i», 1, 3, «i«»1», «|»)», «11|21|31»)

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
		g»)»,
		«>1=a 2= 3=<>1=d 2= 3=<>1=g 2= 3=<»)
m4_test(«m4_define(«m4_func», «>1=$1 2=$2 3=$3<»)m4_applyforeachq(«m4_func», «
		z,
		x,
		y», « % »)»,
		«>1=z 2= 3=< % >1=x 2= 3=< % >1=y 2= 3=<»)
m4_test(«m4_define(«m4_func», «>1=$1 2=$2 3=$3<»)m4_applyforeachq(«m4_func», ««1», «2», «3»», « % »)»,
		«>1=1 2= 3=< % >1=2 2= 3=< % >1=3 2= 3=<»)
m4_test(«m4_define(«m4_func», «>1=$1 2=$2 3=$3<»)m4_dnl
m4_applyforeachq(«m4_func», «««a», «b», «c»», ««d», «e», «f»», ««g», «h», «i»»», « % »)»,
	«>1=a 2=b 3=c< % >1=d 2=e 3=f< % >1=g 2=h 3=i<»)


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

m4_test(«m4_applyforeachqdefine(«««1», «2»», ««3», «4»»», «one=$1 two=$2», «,»)», «one=1 two=2,one=3 two=4»)
m4_test(«m4_applyforeachqdefine(«a, b», «>>$1<<»)», «>>a<<>>b<<»)

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
m4_test(«m4_define(«m4_func», «m4_forloop(«J», 1, $1, «@$1, J%»)»)m4_applyforloop(1, 4, «m4_func»)»,
		«@1, 1%@2, 1%@2, 2%@3, 1%@3, 2%@3, 3%@4, 1%@4, 2%@4, 3%@4, 4%»)

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

m4_test(«m4_applyforloopdefine(1, 3, «arg=$1», « S »)», «arg=1 S arg=2 S arg=3»)
m4_test(«m4_applyforloopdefine(1, 3, «ARG=$1 »)», «ARG=1 ARG=2 ARG=3 »)
m4_test(
		«m4_define(«m4_func», «cnt=$1»)m4_I(
		)m4_applyforloopdefine(1, 3, «m4_applyforloop(1, $1, «m4_func», «,»)», « S »)»,
		«cnt=1 S cnt=1,cnt=2 S cnt=1,cnt=2,cnt=3»)



dnl }}}
dnl {{{ subprocess

m4_define_function(«m4_erun», «m4_esyscmd(m4_shquote($@))»)

m4_define_function(«m4_run», «m4_syscmd(m4_shquote($@))»)

/**
 * @brief Quote the argument according to the shell.
 */
#define m4_shqoute(...)
m4_define(«m4_shquote»,
		«m4_ifelse(
			«$#», 0, «»,
			«$#», 1, «'m4_patsubst(«$1», «'», «'\\''»)'»,
			«m4_shquote(«$1») m4_shquote(m4_shift($@))»)»)
m4_test(«m4_shquote(a)», «'a'»)
m4_test(«m4_shquote(a, b c, d)», «'a' 'b c' 'd'»)

m4_define(«m4_shsplit_sh»,
	«m4_patsubst(
		m4_esyscmd(«printf "%s" »m4_shquote($1)« | xargs printf "«««%s»»,»"»),
		«,$»)»)

dnl }}}
dnl {{{ path

/**
 * @ingroup m4
 * @return The filename from __file__
 */
#define m4_filename(...)
m4_define(«m4_filename», «m4_patsubst(m4___file__, «^.*\(/\|\\\)»)»)

m4_define_function(«m4_write», «m4_syscmd(«printf "%s" »m4_shquote(«$2»)« > »m4_shquote(«$1»)»)

m4_define_function(«m4_append», «m4_syscmd(«printf "%s" »m4_shquote(«$2»)« >> »m4_shquote(«$1»)»)

m4_define_function(«m4_read», «m4_erun(«cat», «$1»)»)

dnl }}}
dnl {{{ math

m4_define_function(«m4_max(number, ...)»,
	«m4_case(«$#»,
		«0», «»,
		«1», «$1»,
		«2», «m4_ifmath(«$1 > $2», «$1», «$2»)»,
		«$0($0($1, $2), m4_shift(m4_shift($@)))»)»)


m4_define_function(«m4_min(number, ...)»,
	«m4_case(«$#»,
		«0», «»,
		«1», «$1»,
		«2», «m4_ifmath(«$1 < $2», «$1», «$2»)»,
		«$0($0($1, $2), m4_shift(m4_shift($@)))»)»)

dnl }}}
dnl {{{ include

#define m4_include_relative »
m4_define_function(«m4_include_relative», «m4_include(m4_patsubst(m4___file__, «/«^/»*$»)/$1)»)

#define m4_include_relative_quiet »
m4_define_function(«m4_include_relative_quiet», «m4_divert(-1)m4_include_relative(«$@»)m4_divert(0)»)

m4_define_function(«m4_included», «()»)

m4_define_function(«m4_include_once(file)»,
	«m4_callif(
		m4_tuple_has(m4_included, «$1»), «1», «m4_fatal(«file $1 included twice»)»,
		«m4_do(
			«m4_define(«m4_included», m4_tuple_push_back(m4_included, «$1»))»,
			«m4_include(«$1»)»,
		)»,
	)»)

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
		m4_ifelse(«$3», «», ««_m4_sEqCoMmA_vArIaBlE»», «$3»),
		«$1»,
		«$2»,
		«m4_ifelse(«$4», «», m4_ifelse(«$3», «», ««_m4_sEqCoMmA_vArIaBlE»», «$3»), «$4»)»,
		«m4_ifelse(«$5», «», «,», «$5»)»)»)
m4_test(«m4_seqcomma(1, 5)», «1,2,3,4,5»)

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
m4_test(«m4_seqdashcomma(1, 5)», «_1,_2,_3,_4,_5»)

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
m4_test(«m4_seqdashcommaX(1, 5, «hello«»X»)», «hello_1,hello_2,hello_3,hello_4,hello_5»)

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
