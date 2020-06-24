
/**
 * @ingroup m4
 * @return The filename from __file__
 */
#define m4_filename(...)
m4_define(`m4_filename~, `m4_patsubst(m4___file__, `^.*\(/\|\\\)~)~)