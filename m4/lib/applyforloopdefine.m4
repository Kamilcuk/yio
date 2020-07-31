
`/**
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
#define m4_applyforloopdefine(start_range, end_range, function_body, separator) ~

m4_define(`m4_applyforloopdefine~, `m4_pushdef(
	`_$0_function~,
	`$3~)m4_forloop(
		`_$0_iterator~,
		`$1~,
		`$2~, 
		`_$0_function(_$0_iterator)~,
		`$4~)m4_popdef(
			`_$0_function~)~)

m4_TEST(`m4_applyforloopdefine(1, 3, `arg=$1~, ` S ~)~)
// PASS_REGULAR_EXPRESSION arg=1 S arg=2 S arg=3
m4_TEST(`m4_applyforloopdefine(1, 3, `ARG=$1 ~)~)
// PASS_REGULAR_EXPRESSION ARG=1 ARG=2 ARG=3
// Recursive calls have to be defined beforehand 
m4_TEST(`m4_define(`func~, `cnt=$1~)m4_applyforloopdefine(1, 3, `m4_applyforloop(1, $1, `func~, `,~)~, ` S ~)~)
// PASS_REGULAR_EXPRESSION cnt=1 S cnt=1,cnt=2 S cnt=1,cnt=2,cnt=3

			
