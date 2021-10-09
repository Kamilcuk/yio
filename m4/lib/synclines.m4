

«/**
 * Synchronize the lines.
 * @param inc Increment the current line number by this value.
 */
#define m4_syncline_in(inc) »
m4_define_function(«m4_syncline_in»,
«#line m4_eval(m4___line__ m4_ifelse(«$1», «», «», « + $1»)) "m4___file__"
»)

«/**
 * Same as m4_syncline_in, but expands to nothing if not debug.
 * @see m4_syncline_in
 * @param inc
 */
#define m4_syncline(inc) »
m4_define_function(«m4_syncline», «m4_ifdef(«m4_DEBUG», «m4_syncline_in($@)»)»)
