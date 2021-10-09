m4_divert(-1)

m4_changequote(`«',`»')

m4_include(lib/lib.m4)

m4_changecom(«/*», «*/»)

m4_include(template_chooser.m4)

/**
 * @defgroup m4 
 * @brief Group with m4 macros.
 */

m4_define(«m4_forloopXcomma», «m4_forloopX(«$1», «$2», «$3», «,»)»)
m4_define(«m4_forloopYcomma», «m4_forloopY(«$1», «$2», «$3», «,»)»)
m4_define(«m4_forargs», «m4_forloopXcomma(«$1», «$2», ««_»X»)»)
m4_define(«m4_fornumbers», «m4_forloopXcomma(«$1», «$2», «X»)»)

m4_divert(0)m4_dnl
