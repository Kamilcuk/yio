m4_changequote(,)
m4_changequote(«, »)
m4_changecom(«/*», «*/»)

m4_define(«m4_to_m»,
	«m4_ifelse(
		«$1», «», «»,
		«m4_define(«m_$1», «m4_$1»)$0(m4_shift($@))»)»)

m4_to_m(m4_patsubst(
			«define ifelse shift eval decr incr»,
			«\s*\(\S*\)\s*», «««\1»»»))
