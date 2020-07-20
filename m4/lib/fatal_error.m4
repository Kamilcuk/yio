
/**
 * @param message Message to print.
 * @param ... List of any arguments to print
 * @ingroup m4
 * @brief Just print an error and exits m4 with an error.
 */
#define m4_fatal_error(message, ...)

m4_define(`m4_fatal_error~, `m4_errprint(m4___file__:m4___line__`: error: $*
~)m4_m4exit(`1~)~)

m4_define(`m4_fatal~, `m4_errprint(m4___file__:m4___line__`: error: $*
~)m4_m4exit(`1~)~)

