
/**
 * @def m4_define(param)
 *
 * Remove leadig newlines from param expansion.
 */
m4_define(«m4_removeln», «m4_patsubst($@, «^
*»)»)

/**
 * @def m4_define_function(name, [value])
 *
 * https://www.gnu.org/software/m4/manual/m4-1.4.14/html_node/Composition.html
 * But also remove leading empty newlines in if any. Just so that m4_dnl could be removed.
 */
#define m4_define_function(...)
m4_define(«m4_define_function», «m4_ifelse(«$#», «0», ««$0»»,
	«_$0(«$1», «$2», «$»«#», «$»«0»)»)»)
m4_define(«_m4_define_function», «m4_define(«$1»,
	«m4_ifelse(«$3», «0», ««$4»», «m4_removeln(«$2»)»)»)»)
