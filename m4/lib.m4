m4_divert(-1)

m4_changequote(`«',`»')

m4_include(lib/lib.m4)

m4_changecom(«/*», «*/»)

m4_include(template_chooser.m4)

/**
 * @defgroup m4 
 * @brief Group with m4 macros.
 */

m4_define_function(«m4_forloopXcomma», «m4_forloopX(«$1», «$2», «$3», «,»)»)
m4_define_function(«m4_forloopYcomma», «m4_forloopY(«$1», «$2», «$3», «,»)»)
m4_define_function(«m4_forargs», «m4_forloopXcomma(«$1», «$2», ««_»X»)»)
m4_define_function(«m4_fornumbers», «m4_forloopXcomma(«$1», «$2», «X»)»)


/**
 * @def m4_removeln(param)
 *
 * Remove leading newlines from param expansion.
 */
m4_define(«m4_removeln», «m4_patsubst($@, «^
*»)»)

m4_define_function(«m4_silentinclude», «m4_divert(-1)m4_include($@)m4_divert(0)m4_dnl »)

m4_define_function(«m4_applysync», «m4_applyforeachdefine(«$1», «
m4_syncline(1)m4_dnl $2
m4_syncline()»)m4_dnl »)


m4_divert(0)m4_dnl
