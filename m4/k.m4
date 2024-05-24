m4_divert(-1)

m4_changequote(`«',`»')

m4_changecom(«/*», «*/»)

// Order matters
m4_include(k/define_function.m4)
m4_include(k/define_name.m4)

// Include all files excluding this file and the one above
m4_include(k/applyforeach.m4)
m4_include(k/applyforeachdefine.m4)
m4_include(k/applyforeachq.m4)
m4_include(k/applyforeachqdefine.m4)
m4_include(k/chop.m4)
m4_include(k/esysexec.m4)
m4_include(k/fatal_error.m4)
m4_include(k/filename.m4)
m4_include(k/foreach.m4)
m4_include(k/forloop.m4)
m4_include(k/forloopdash.m4)
m4_include(k/ignore.m4)
m4_include(k/include_relative.m4)
m4_include(k/quote.m4)
m4_include(k/seq.m4)
m4_include(k/shell_qoute.m4)
m4_include(k/synclines.m4)
m4_include(k/applyforloopdefine.m4)
m4_include(k/applyforloop.m4)

m4_divert(0)m4_dnl
