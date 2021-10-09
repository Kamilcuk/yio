
«/**
 * Chooser proper section to include depending on current templating mode
 */
#define m4_template_chooser(NORMAL, WIDE, UNICODE) »
m4_define_function(«m4_template_chooser», «m4_dnl;
m4_ifelse(«$#», «4», «», «m4_dnl;
m4_fatal_error(«m4_template_chooser: Wrong number of arguments $#»)»)m4_dnl;
m4_ifdef(«_yIO_TYPE_YIO»,  «$1», «m4_dnl;
m4_ifdef(«_yIO_TYPE_YWIO», «$2», «m4_dnl;
m4_ifdef(«_yIO_TYPE_YC16IO», «$3», «m4_dnl;
m4_ifdef(«_yIO_TYPE_YUIO», «$4», «m4_dnl;
m4_fatal_error(«m4_template_chooser used in non-template context»)»)»)»)»)»)

«
#define m4_template_chooser2 »
m4_define_function(«m4_template_chooser2», «m4_dnl;
m4_ifelse(«$#», «3», «», «m4_dnl;
m4_fatal_error(«m4_template_chooser: Wrong number of arguments $#»)»)m4_dnl;
m4_template_chooser(«$1», «$2», «$3», «$3»)»)