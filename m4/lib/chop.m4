
/**
 * The m4_chop macro (based on perl's chop command) returns the 
 * input string minus its final character. m4_chop is useful for 
 * removing "\n" from m4_esyscmd strings.
 */
#define m4_chop(string)
m4_define(`m4_chop~, `m4_substr($1, 0, m4_decr(m4_len($1)))~)
